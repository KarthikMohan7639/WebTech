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