import React from 'react';
import 'bootstrap/dist/css/bootstrap.css';
import '../styles/Login.css';
import { useState } from 'react';
import logo from '../Logo.png'
import 'bootstrap-icons/font/bootstrap-icons.css';
import { Button } from 'bootstrap';
import { Link } from 'react-router-dom';

const alertPlaceholder = document.getElementById('liveAlertPlaceholder')
const appendAlert = (message, type) => {
  const wrapper = document.createElement('div')
  wrapper.innerHTML = [
    `<div class="alert alert-${type} alert-dismissible" role="alert">`,
    `   <div>${message}</div>`,
    '   <button type="button" class="btn-close" data-bs-dismiss="alert" aria-label="Close"></button>',
    '</div>'
  ].join('')

  alertPlaceholder.append(wrapper)
}

const alertTrigger = document.getElementById('liveAlertBtn')
if (alertTrigger) {
  alertTrigger.addEventListener('click', () => {
    appendAlert('Inicio de sesion correcto!', 'success')
  })
}


function Login(){
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
        <i class="bi bi-house-door bi-6x"  style={{fontSize:60, color:"#e0caa1"}}  width="200px" ></i>
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
          type="password" 
          value={password}
          onChange={(e) => setPassword(e.target.value)}
        />
    </form>
    {/* <Link to = "/Chat"> */}
    <div id="liveAlertPlaceholder"></div>
          <button type="button" className="btnLogin" id="liveAlertBtn">Iniciar Sesion</button>
          {/* </Link> */}
    </header>
</div>
    )
}

export default Login;