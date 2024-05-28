import React, { useState } from 'react';
import 'bootstrap/dist/css/bootstrap.css';
import '../styles/Register.css';
import logo from '../Logo.png';
import 'bootstrap-icons/font/bootstrap-icons.css';
import { Link, useNavigate } from 'react-router-dom';

function Register() {
  const [name, setName] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const [success, setSuccess] = useState("");
  const navigate = useNavigate();

  // Fetch existing users from local storage
  const getUsers = () => {
    const users = localStorage.getItem('users');
    return users ? JSON.parse(users) : [];
  };

  // Handle registration
  const handleRegister = (event) => {
    event.preventDefault();
    const users = getUsers();

    // Check if username is already taken
    const userExists = users.some(user => user.Username === name);
    if (userExists) {
      setError('Username is already taken');
      return;
    }

    // Add new user to the list
    const newUser = { Username: name, Password: password };
    users.push(newUser);
    localStorage.setItem('users', JSON.stringify(users));

    setSuccess('Registration successful! Redirecting to login...');
    setError('');

    // Redirect to Login after 2 seconds
    setTimeout(() => navigate('/Login'), 2000);
  };

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

          <div className=''></div>
          <input
            className='userInput passwordInput'
            placeholder='Password'
            type="password"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
          />
          <button type="submit" className="btnRegister2">Registrarse</button>
        </form>
      </header>
    </div>
  );
}

export default Register;
