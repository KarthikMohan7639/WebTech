console.log("funciton in javascript!")

//how to write a function

function display() {
 console.log("I'm a function!")   
}
display()

//function without parameter
// function add(){
//     let a=10;
//     let b=20;
//     console.log(`the addition of two numbers ${a} and ${b} is ${a+b}`);   
// }
// add()

//function with parameter
function add(a,b){
    console.log(a+b);  
}
add(10,20)

//function with return

function multiply(a,b) {
    return a*b;
}
let res=multiply(10,3)
console.log(res)

let a=10;
let b=20;
console.log(`before swapping a:${a} and b:${b}`)
function swap(){
    let temp=a
    a=b
    b=temp
}
swap()
console.log(`after swapping a:${a} and b:${b}`)

//annonymous function
//the function does not have any name is called annonymous function.

// 3. function with expression
let hello=function(){
    console.log("hello how are you?")
}
hello()