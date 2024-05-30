import React, { useState, useEffect, useCallback } from 'react';
import 'bootstrap/dist/css/bootstrap.css';
import '../styles/Login.css';
import logo from '../Logo.png';
import 'bootstrap-icons/font/bootstrap-icons.css';
import { Link, useNavigate } from 'react-router-dom';
import { useWebSocket } from '../components/WebSocketConnection';

function Login() {
  const [name, setName] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const [userData, setUserData] = useState(null); // Variable de estado para guardar los datos del usuario
  const navigate = useNavigate();
  const { sendMessage, subscribe, unsubscribe, startSync } = useWebSocket();

  const handleLogin = useCallback((event) => {
    event.preventDefault();
    console.log("Sending login message:", { username: name, password: password });
    sendMessage({ action: 'login', data: { username: name, password: password } });
  }, [name, password, sendMessage]);

  useEffect(() => {
    const handleServerMessage = (message) => {
      console.log("Received server message:", message);
      if (message.action === 'auth') {
        if (message.data.status === 'Success') {
          console.log("Login successful, saving data and navigating to /Chat");
          const userInfo = {
            userId: message.data.user_id,
            publicKey: message.data.public_key
          };
          setUserData(userInfo);
          
          console.log("User Data:", userInfo);
          // startSync(); // Iniciar la sincronización después de un inicio de sesión exitoso
          navigate('/Chat');
        } else {
          setError(message.data.return);
        }
      } else if (message.action === 'error') {
        setError(message.data.return);
      }
    };

    console.log("Subscribing to server messages");
    subscribe(handleServerMessage);
    return () => {
      console.log("Unsubscribing from server messages");
      unsubscribe(handleServerMessage);
    };
  }, [subscribe, unsubscribe, navigate]);

  return (
    <div className='Login'>
      <header className="App-header">
        <a className='homeBtn' href="#!" role="button">
          <Link to="/Menu">
            <i className="bi bi-house-door bi-6x" style={{ fontSize: 60, color: "#e0caa1" }}></i>
          </Link>
        </a>
        <img className="logo4" src={logo} alt="Logo" />
        {error && <div className="alert alert-danger">{error}</div>}
        <form onSubmit={handleLogin} style={{ padding: 80 }}>
          <div className='userNameLogin'>
            <label>Username</label>
            <input
              className='userInput'
              placeholder='Username'
              type="text"
              value={name}
              onChange={(e) => setName(e.target.value)}
            />
          </div>

          <div className='passwordLogin'>
            <div>Password</div>
            <input
              className='userInput passwordInput'
              placeholder='Password'
              type="password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
            />
          </div>
          <button type="submit" className="btnLogin4">Iniciar Sesión</button>
        </form>
      </header>
    </div>
  );
}

export default Login;
