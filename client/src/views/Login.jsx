import React, { useState, useEffect } from 'react';
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
  const sendMessage = useWebSocket();

  const getUsers = () => {
    const users = localStorage.getItem('users');
    return users ? JSON.parse(users) : [];
  };

  const handleLogin = (event) => {
    event.preventDefault();
    const users = getUsers();
    const user = users.find(user => user.Username === name && user.Password === password);

    if (user) {
      setError('');
      sendMessage({ action: 'login', data:{username: user.Username, password: user.Password} });
    } else {
      setError('Invalid username or password');
    }
  };

  const handleServerMessage = (message) => {
    if (message.action === 'auth') {
      if (message.data.status === 'Success') {
        navigate('/Chat');
      } else {
        setError(message.data.return);
      }
    } else if (message.action === 'error') {
      setError(message.data.return);
    }
  };

  useEffect(() => {
    const handleMessage = (message) => {
      handleServerMessage(message);
    };
    sendMessage.subscribe(handleMessage);
    return () => {
      sendMessage.unsubscribe(handleMessage);
    };
  }, [sendMessage]);

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
