#include "fuzz_assign_operation.h"

namespace fuzz 
{
    template <typename primitive_t>
    Identifier getId(primitive_t) {
        throw std::exception("LHS of := is not an identifier");
    }
    
    template<>
    Identifier getId(Identifier id) {
        return id;
    }

    void AssignOperation::execute(EvaluationContext & context) const
    {
        if (auto* pe = dynamic_cast<PrimitiveExpression const*>(lhs.get())) 
        {
            auto id = std::visit([](auto p) {
                return getId(p);
            }, pe->primitive);

            auto rhs_eval = rhs->evaluate( context );
            context.setVariable(id, rhs_eval);
        }
        else
        {
           throw std::exception("LHS of := is not an primitive expression");
        }
    }
}

