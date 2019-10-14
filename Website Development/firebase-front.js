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

//for signup
const signup_name = document.getElementById('signup-name');
const signup_surname = document.getElementById('signup-surname');
const signup_role = document.getElementById('signup-role');
const signup_email = document.getElementById('signup-email');
const signup_pwd = document.getElementById('signup-pwd');
const signup_confirm = document.getElementById('signup-confirm');
const signup_btn = document.getElementById('signup-btn');
//for signin
const signin_email = document.getElementById('signin-email');
const signin_pwd = document.getElementById('signin-pwd');
const signin_btn = document.getElementById('signin-btn');


//Login Event
signin_btn.addEventListener('click', e => {
    const si_email = signin_email.value;
    const si_pwd = signin_pwd.value;
    const auth = firebase.auth();
    
    const prom = auth.signInWithEmailAndPassword(si_email, si_pwd);
    prom.catch(e => console.log(e.message));
});

//signup Event
signup_btn.addEventListener('click', e => {
    if (signup_name.value == "Francis" && signup_surname.value == "Francis"){
        const su_pwd = signup_pwd.value;
        //TODO: check for real email
        const su_email = signup_email.value;

        const auth = firebase.auth();

        const prom = auth.createUserWithEmailAndPassword(su_email, su_pwd);
        prom.catch(e => console.log(e.message));
    } 
});

firebase.auth().onAuthStateChanged(firebaseUser => {
    if(firebaseUser){
        window.location = 'ind.html';
    }else {
        console.log('Not logged in');
    }
});



//on auth state. meaning if logged in, 
//user info filled in firebaseUser, null on logged out