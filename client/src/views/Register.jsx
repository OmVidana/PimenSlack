import React, { useState, useEffect, useCallback } from 'react';
import 'bootstrap/dist/css/bootstrap.css';
import '../styles/Register.css';
import logo from '../Logo.png';
import 'bootstrap-icons/font/bootstrap-icons.css';
import { Link, useNavigate } from 'react-router-dom';
import { useWebSocket } from '../components/WebSocketConnection';
import { encrypt } from '../components/Encryption';

function Register() {
  const [name, setName] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const [success, setSuccess] = useState("");
  const navigate = useNavigate();
  const { sendMessage, subscribe, unsubscribe, encryptionKeys } = useWebSocket();

  const handleRegister = useCallback((event) => {
    event.preventDefault();
    if (encryptionKeys) {
      const encryptedName = encrypt(name, encryptionKeys.n, encryptionKeys.e);
      const encryptedPassword = encrypt(password, encryptionKeys.n, encryptionKeys.e);
      sendMessage({ action: 'register', data: { username: encryptedName, password: encryptedPassword } });
    } else {
      setError('Encryption keys not available.');
    }
  }, [name, password, sendMessage, encryptionKeys]);

  useEffect(() => {
    const handleServerMessage = (message) => {
      if (message.action === 'register') {
        if (message.data.status === 'Success') {
          setSuccess('Registration successful! Redirecting to login...');
          setError('');
          setTimeout(() => navigate('/Login'), 2000);
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
        {success && <div className="alert alert-success">{success}</div>}
        <form onSubmit={handleRegister} style={{ padding: 80 }}>
          <div className='userNameRegister'>
            <div>Username</div>
            <input
              className='userInput'
              placeholder='Username'
              type="text"
              value={name}
              onChange={(e) => setName(e.target.value)}
            />
          </div>

          <div className='passwordRegister'>
            <div>Password</div>
            <input
              className='userInput passwordInput'
              placeholder='Password'
              type="password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
            />
          </div>
          <button type="submit" className="btnRegister2">Registrarse</button>
        </form>
      </header>
    </div>
  );
}

export default Register;