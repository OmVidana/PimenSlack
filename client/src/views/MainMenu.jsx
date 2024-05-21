import '../styles/MainMenu.css';
import 'bootstrap/dist/css/bootstrap.css';
import logo from '../Logo.png'
import Login from './Login';
import { Link } from 'react-router-dom';

import { useState } from 'react'; 
function MainMenu() {

  return (
    <div className="App">
      <header className="App-header">
        <div>
          <img className = "logo2" src={logo}></img>
        </div>
        <Link to="/Login">
          <button type="button" className="btnLogin" >Iniciar Sesion</button>
          </Link>
          <Link to="/Register">
              <button type="button" className="btnRegister">Registrarse</button>
              </Link>

      </header>
    </div>
  );
}

export default MainMenu;
