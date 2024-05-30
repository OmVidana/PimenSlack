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
  const navigate = useNavigate();
  const { sendMessage, subscribe, unsubscribe } = useWebSocket();

  const handleLogin = useCallback((event) => {
    event.preventDefault();
    sendMessage({ action: 'login', data: { username: name, password: password } });
  }, [name, password, sendMessage]);

  useEffect(() => {
    const handleServerMessage = (message) => {
      if (message.action === 'auth') {
        if (message.data.status === 'Success') {
          const userInfo = {
            userId: message.data.user_id,
            publicKey: message.data.public_key
          };
          navigate('/Chat', { state: { user: userInfo } });
        } else {
          setError(message.data.return);
        }
      } else if (message.action === 'error') {
        setError(message.data.return);
      }
    };

    subscribe(handleServerMessage);
    return () => {
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
