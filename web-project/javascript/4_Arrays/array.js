//In javascript array is a linear datastructre that stores group of elements together

//arr
let arr=[1,2,3,4];
console.log(arr);

//how to access arr elements
console.log(arr[1]);

//1.Array methods
//1.push()
//this method is used to add nay elemetn at end of the array.

let students=["karthik","mohan","pant"]
console.log(students)

students.push("sachin")

console.log(students);

//2.pop()
//this method is used to remove the last element from the array

let players=['gambhir','virat','shewag']

players.pop()

console.log(players)

//3.unshift
// this method is used to add any element at the starting of the array and it will return the length of the modified aray.
let serials=['vani rani','chandralekha']

serials.unshift('singapenne')

//4.shift()
//this method is used to remove first element in a array

let movies=['robo','leo','gpat','dj']

movies.shift()

console.log(movies)

//5. includes()
//it is used to check given element is present or not in the array

//if it is present returns true otherwise it will return false.

console.log(movies.includes('robo'))
console.log(movies.includes('dj'))

//6.indexOf()

//it is used to know the index of the given element.

let webseries=['loki','dark','breaking bad','money heist']

console.log(movies.indexOf('dj'))

console.log(webseries.indexOf('dark'));

// 7.concat()
// concat() method is used to combine two or more than two arrays and it will return one new array.

let arr2 = [10,20,30]

let arr3 = [40,50,60]

let combineArr=arr2.concat(arr3,'hii','world')

console.log(combineArr)

//5. join()
//it is used to convert any array into string.
let charArr=['h','e','l','l','o']
console.log(charArr.join())


//reverse()
//this is used to reverse the original array in reverse order

let arr4=[1,2,3,4,5]

console.log(arr4.reverse())

console.log(arr4)

//splice()
//this mehtod is used to remove add modify the array elements 
//it can take 3 parameter(startIndex,deletecount,replacementValue)
//this method will modify the original array
let arr5=[10,20,30,40,50]
arr5.splice(1,2)
console.log(arr5)

arr5.splice(2,0,25)
console.log(arr5)

//Higher order function forEach()
//foreach method is a higher order function which is used for traversing the array.
//it can take one callback function where it can take 3 parameters (element,index,array)

let numbers=[10,20,30,40,50];

numbers.forEach((ele,index,array)=>{
console.log(ele,index,array)
});

let arr8=[];
numbers.forEach((ele)=>{
    arr8.push(ele+100);
});

console.log(arr8);
