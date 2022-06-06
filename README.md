# FUZZ: "Better than Javascript"

(Under active development - very incomplete)

Fuzz is a new language that is a Functional Object-Oriented (This style referred henceforth to as "foo"), and does neither functional nor objecto oriented programming well.

## Fuzzy Matching: Automatic error detection

Fuzz will (Soon™) lookup all variable names via fuzzy matching, thus allowing many programming errors to be fixed seamlessly at runtime.

```
let myCorrectlySpeltVariableName := 5;
let somethingElse := myCorrectlySpelltVariableName + 3;
// fuzzy-matching deduces myCorrectlySpelltVariableName 
// is a misspelling of    myCorrectlySpeltVariableName 
// and somethingElse now contains the value 8
```

Fuzz is weakly-typed (in every sense of the word), so whatever type you end up getting, fuzz won't complain! (Even if you want it to!)

## Functional Programming: Currying and Hyper-currying™

All functions are lambdas:
```
let f := /x/y/z{ return x + y + z };
```

Parameter-packs may be passed to functions as arrays, using the `.` operator:
```
let arguments := [ 2, 3, 4 ];
let result = f.arguments;
// result now contains the value 9 (=2+3+4)
```

Incomplete parameter packs may also be passed - the values being automatically bound in - with the result being a curried function, just like in incredibly popular functional languages such as Haskell
```
let g  := f . [ 2, 3 ];
// g now contains a function, equivalent to:
// let g := /z{ return 3+4+z; }
result = g . 4; // If we just have 1 argument, we do not need to pass it as an array;
// result now contains the value 9 as in the previous example
```

But what if we pass too many arguments? Then the extra parameters will be Hyper-curried™* into the result of the completed function.
*This is definitely a real word.

```
let h := /x/y{ 
  let new_lambda := /xx/yy/z{ return x+y+z; }
  return new_lambda.[x,y]
};

let result = h.[ 2, 3, 4 ];
// result contains the value 9, as the above expression is equivalent to:
let result = (h.[2,3]).[4];
```

## Object Oriented Programming: Somehow this works

If you don't put a return statement in a function, it's probably not a function, right?

```
let dog := /name/age {
  let name_ := name;
  let age_ := age;
};

let my_dog := dog.[ "Rover", 12 ];
// dog didn't return anything, so consider this an object? Maybe?

let dogs_name := dog."name_";
// here dogs_name contains the value "Rover"
```

## FAQ:
### I didn't see anything about any control-flow statements in the language. Does that mean there aren't any?
Yeah. I reckon probably you can implement this stuff within the language as it stands, and I've got my best brain-cells working hard to figure out how. Once I solve this, and add support for modules, something resembling if-statements may end up in the standard library.

### What's the performance like in this language?
Don't think too hard about it.

### How does fuzz interact with anything else? It doesn't appear to have any IO features.
🤷

### Can I try out fuzz?
Fuzz uses my custom build system [AndrewBoyd/Eiffel]() to create the Visual Studio project files. As well as having some external dependencies such as [foonathan/Lexy
](https://github.com/foonathan/lexy). If you want to attempt to navigate that, then be my guest. I offer you no support in this matter. 

### Is fuzz a joke?
Yes and no. The fuzzy matching, yes. Also many of the language features are designed around allowing all operators to take as many different types of operands as possible, to maximise carnage when the fuzzy-matching inevitibly works against you.
However, the more I began to design the language, the more I wanted to try out creating experimental language features, such as the so-called Hyper-currying, and using unified syntax for objects and functions. 
Fuzz is a bad language - but it has cool features. Discovering why the bad features are bad, and the good features are good has been a rollercoaster.

#### Notice
All code copyright Andrew Boyd. For entertainment purposes only. You may not redistribute any of this code in any form.
