const arr=[12,10,2,4,11];

// let secondMax=(arr)=>{
//     let firstMax=arr[0];
//     let secMax=0;
//     for(let i=1;i<=arr.length;i++){
//         if(firstMax>arr[i]){
//             secMax=firstMax;
//             firstMax=arr[i];
//         }
//     }
// };

const findSecondLargest=(arr)=>{
    if(arr.length<2) return -1;
    let first=-Infinity;
    let second=-Infinity;
    arr.forEach(num => {
        if(num>first){
            second=first;
            first=num;
        }
        else if (num>second && num!=first){
            second=num;
        }
    });
    return second===-Infinity?"no second largest":`second largest is ${second}`;
};
console.log(findSecondLargest(arr));
