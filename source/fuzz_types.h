#pragma once
#include <utility/value_ptr.h>
#include <optional>
#include <variant>
#include <vector>
#include <string>
#include <any>
#include <map>
#include <set>

namespace fuzz
{
	using string_t = std::u8string;
	
	class EvaluationContext;

	struct Expression_base;
	using Expression = stdish::value_ptr<Expression_base>;
	struct PrimitiveExpression;

	enum class BinaryOperator {
		add, 
		subtract, 
		multiply, 
		divide, 
		modulo,
		less_than, 
		less_than_or_equal, 
		greater_than, 
		greater_than_or_equal, 
		equal, 
		not_equal,
		logical_and, 
		logical_or,
		evaluate,
	};

	enum class PrefixKeyword {
		return_,
		temp, 
		let,
	};

	struct Assignment_t {};
	struct Evaluation_t {};

	// PRIMITIVES

	using Boolean = bool;
	using Number = double;
	using String = string_t;
	using Array = std::vector<Expression>;
	using Block = struct {
		std::vector<Expression> statements;
	};
	using Identifier = struct {
		string_t id;
	};
	using Lambda = struct {
		std::vector<Identifier> free_parameters;
		Block block;
	};

	struct Object;

	using Primitive = std::variant<
		Boolean,
		Number,
		String,
		Array,
		Lambda,
		Object,
		Identifier
	>;

	struct Object {
		std::map<string_t, Primitive > values;
	};

	// EXPRESSIONS

	struct Expression_base{
		virtual ~Expression_base() = default;
		
		virtual Primitive evaluate(EvaluationContext const &) const
		{
			throw "Expression does not return a value";
		}
		
		virtual void execute(EvaluationContext&) const
		{
			throw "Expression is not a statement";
		}
	};

	class BinaryOperation : public Expression_base 
	{
	public:
		explicit BinaryOperation(Expression lhs, BinaryOperator op, Expression rhs) 
			: Expression_base()
			, operation_(op)
			, lhs_(std::move(lhs))
			, rhs_(std::move(rhs))
		{}

		Primitive evaluate(EvaluationContext const&) const override;

		BinaryOperator operation_;
		Expression lhs_;
		Expression rhs_;
	};

	struct KeywordOperation : public Expression_base
	{
		explicit KeywordOperation(PrefixKeyword kw, Expression rhs)
			: Expression_base()
			, keyword(kw)
			, expr(std::move(rhs)) 
		{};

		PrefixKeyword keyword;
		Expression expr;
	};

	struct AssignOperation : public Expression_base
	{
		explicit AssignOperation(Expression l, Expression r)
			: Expression_base()
			, lhs(std::move(l))
			, rhs(std::move(r))
		{};

		Expression lhs;
		Expression rhs;
	};

	struct Evaluation : public Expression_base
	{
		explicit Evaluation(Expression lhs, Expression rhs)
			: Expression_base()
			, to_evaluate(std::move(lhs))
			, parameter_pack(std::move(rhs))
		{};

		Expression to_evaluate;
		Expression parameter_pack;
	};

	struct PrimitiveExpression : public Expression_base 
	{
		Primitive primitive;

		Primitive evaluate(EvaluationContext const&) const override;
	};

	using Program = Block;


	class EvaluationContext 
	{
	public:
		Primitive& get(Identifier id);
		Primitive const & get(Identifier id) const;

	private:
		std::list<Object> context_;
		std::set<Identifier> temp_names_;
	};

}
