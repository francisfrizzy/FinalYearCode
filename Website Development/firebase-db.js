// Your web app's Firebase configuration
var firebaseConfig = {
apiKey: "AIzaSyDIdkNqQweE_nYE74hExtKQvOepSesNsdA",
authDomain: "slbic-system-demo.firebaseapp.com",
databaseURL: "https://slbic-system-demo.firebaseio.com",
projectId: "slbic-system-demo",
storageBucket: "",
messagingSenderId: "776366340310",
appId: "1:776366340310:web:6b00012e48a21d78b2f177",
measurementId: "G-582F0TF9ES"
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);
firebase.analytics();


const preObject = document.getElementById('object');
const tableObj = document.getElementById('table1');
const logout_btn = document.getElementById('logout-btn');


const dbRefObject = firebase.database().ref('Hubs');
dbRefObject.on('value',gotData, errData);

logout_btn.addEventListener('click', e => {
    firebase.auth().signOut();
});

firebase.auth().onAuthStateChanged(firebaseUser => {
    if(!firebaseUser){
        window.location = 'frontpage.html';
    }
});

function gotData(data){
    var clear = document.querySelectorAll(".listing");
    var clear2 = document.querySelectorAll(".listing2");
    for (var i = 0; i < clear.length; i++){
        clear[i].remove();
        clear2[i].remove();
    }
    
    var scores = data.val();
    var keys = Object.keys(scores);
    //console.log(keys);
    for (var i=0; i < keys.length; i++){
        var content = '';
        var content2 = '';
        var k = keys[i];
        var hubID = scores[k].hubID;
        var nodeID = scores[k].nodeID;
        var current = scores[k].current;
        var motion = scores[k].pir;
        var temperature = scores[k].temperature;
        var lampStatus = scores[k].lampStatus;
        
        if (lampStatus == 1){lampStatus = 'ON';}
        else {lampStatus = 'OFF';}
        
        content += '<tr class="listing">';
        content += '<td>'+hubID+'</td>'; //column1
        content += '<td>'+nodeID+'</td>';//column2
        content += '<td>'+temperature+'</td>';//column3
        content += '<td>'+current+'</td>';//column4
        content += '<td>'+motion+'</td>';//column5
        content += '</tr>';
        
        content2 += '<tr class="listing2">';
        content2 += '<td>'+nodeID+'</td>';//column1
        content2 += '<td>'+lampStatus+'</td>';//column2
        content2 += '</tr>';
        $('#table1').append(content);
        $('#table2').append(content2);
    }
}

function errData(err){
    console.log('Error');
    console.log(err);
}