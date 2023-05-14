var firebase = require('firebase')
    // datos de firebase
var config = {

    apiKey: "AIzaSyBmWIP0VDG45KEZFBabrFjlKzI8sXyyvvQ",
    authDomain: "idcheck-a9646.firebaseapp.com",
    databaseURL: "https://idcheck-a9646-default-rtdb.firebaseio.com",
    projectId: "idcheck-a9646",
    storageBucket: "idcheck-a9646.appspot.com",
    messagingSenderId: "560271868131",
    appId: "1:560271868131:web:9c844cd3dd096dd9cc45d6"
};

const fire = firebase.initializeApp(config);
module.exports = fire;