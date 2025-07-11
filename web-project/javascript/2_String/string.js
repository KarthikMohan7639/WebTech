//how to declare string
let str1='first string'
let str2='second string'
let str3=`third string`

console.log(str1);
console.log(str2);
console.log(str3);


console.log(typeof str1);
console.log(typeof str2);
console.log(typeof str3);

//how to know the length of the string
console.log(str1.length)

//1.toUpperCase()
//this method is used to convert any string into uppercase and it will return one new string.

let myname="tharun"
let upper=myname.toUpperCase();

console.log(myname.toUpperCase())

console.log(upper);

console.log(myname);


//2.toLowerCase()
// this method is used to convert any string into lowercase and it will return one new string
let addresse='CHENNAI'
console.log(addresse.toLowerCase());
let lower=addresse.toLowerCase();
console.log(lower);



// 3. ! indexOf()
//it is used to know the index of the given character from the string
// it will take the first occurence of the given character.
//if the character is nor present it will give -1.

console.log(addresse.indexOf('N')) //3
console.log(addresse.indexOf('n')) //-1

// 4. lastIndexOf()
// it will give the last occurence index numver of the given character from the string.

console.log(addresse.lastIndexOf('N'))

// 5. charAt()
// it is used to get the character from the string at specified index
console.log(addresse.charAt(2))

// 6. includes()
//this method is used to know whether the given string is present or not
// if it is present it will return true otherwise it will return false.

console.log(addresse.includes('n'))

// trim()
//it is used to remove trailing and leading whitespaces from the string.
let mobileName=' samsung'

console.log(mobileName)
console.log(mobileName.length);

let mobileName2=mobileName.trim();

console.log(mobileName2);
console.log(mobileName2.length);

console.log(mobileName)

//split()
//it is used to split the string into multiple characters or string array based on arguments passed to the function

let wish='good morning everyone'

let arr=wish.split(" ")  // here only one element will be present

console.log(wish.split()) // here only one element will be present
console.log(wish.split(' '))// splits the string based on spaces

console.log(arr)


// ! replace()
//it will replace the passed string with new string

let information="I'm from bangalore";
let replacedString=information.replace("bangalore","chennai")

console.log(replacedString);

//replaceAll()
//it will replace all ocrrusrances of the given string

let replce2=information.replaceAll("a","@")
console.log(replce2)

//concat()
//it is used to concatenate (add or combine) two strings

let str4='good afternoon'
let usernam='virat'

let newStr=str4.concat(" ",usernam,' how are you?')
console.log(newStr)

// slice()
// it is used ot extract one string from another string.
//it will two parameters(startIndex, endIndex)
//it will not include the endIndex value.

let str5="hello how are you"
console.log(str5.slice(1,4))

console.log(str5.slice(6));

console.log(str5.slice(-7,0)) //no output

console.log(str5.slice(-5,0))

console.log(str5.slice(-3,-1))

//substring()

// substring method is used extract the new string from the original string 
// it has two parameters startIndex and endIndex
//it doesn't take negative parameters

 console.log(str5.substring(1,4));//ell

 console.log(str5.substring(6));

 console.log(str5.substring(-7));

 console.log(str5.substring(4,1))

// String Interpolation

let num1=10
let num2=20

let add=num1+num2

console.log(`the addition of ${num1} and ${num2} is ${add}`);

let num3=20
let num4=10
console.log(`the  subtraction of ${num3} and ${num4} is ${20-10} `)
 
 let string='karthik is a male gender'
 console.log(string.toUpperCase())
 console.log(string.toLowerCase());
 console.log(string.indexOf('s'));
 console.log(string.lastIndexOf('s'));
 console.log(string.length);
 console.log(string.trim())
 console.log(string.replace('is',':'))
 console.log(string.replaceAll(string,"hello how are you?"))
let  strarr=string.split(" ")
console.log(strarr)
 console.log(string.concat(" ","hello world"))
 
 

