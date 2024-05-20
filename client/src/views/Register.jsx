import React from 'react';
import 'bootstrap/dist/css/bootstrap.css';
import '../styles/Register.css';
import { useState } from 'react';
import logo from '../Logo.png'
import 'bootstrap-icons/font/bootstrap-icons.css';
import { Button } from 'bootstrap';
import { Link } from 'react-router-dom';


function Register(){
    const [name, setName] = useState("");
    const [password, setPassword] = useState("");

    const handleSubmitName = (event) => {
        event.preventDefault();
        alert(`The name you entered was: ${name}`)
      }

      const handleSubmitPassword = (event) => {
        event.preventDefault();
        alert(`The password you entered was: ${password}`)
      }

    return(
<div className='Login'>
    <header className="App-header">
    <a data-mdb-ripple-init class="btn btn-primary" className='homeBtn' href="#!" role="button">
    <Link to="/Menu">
        <i class="bi bi-house-door bi-6x"  style={{fontSize:60, color: "#e0caa1"}}  width="200px" ></i>
        </Link>
        </a>
    
    <img className = "logo" src={logo}></img>
    <form onSubmit={handleSubmitName} style={{padding:80}}>
        <input 
        className='userInput'
        placeholder='Username'
          type="text" 
          value={name}
          onChange={(e) => setName(e.target.value)}
        />
    </form>

    <form onSubmit={handleSubmitPassword} className='passwordInput'>
        <input 
          className='userInput'
         placeholder='Password'
          type="text" 
          value={password}
          onChange={(e) => setPassword(e.target.value)}
        />
    </form>
          <button type="button" className="btnRegister" >Registrarse</button>
    </header>
</div>
    )
}

export default Register;