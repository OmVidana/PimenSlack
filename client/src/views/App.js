import React from 'react';
import 'bootstrap/dist/css/bootstrap.css';
import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import MainMenu from '../views/MainMenu';
import Login from '../views/Login';
import MainPage from '../views/MainPage';
import Register from "../views/Register";
import Chat from "../views/Chat";
import { WebSocketProvider } from '../components/WebSocketConnection';

function App() {
  return (
    <WebSocketProvider>
      <Router>
        <div className="body">
          <Routes>
            <Route path='/' exact element={<MainPage />} />
            <Route path='/Menu' exact element={<MainMenu />} />
            <Route path="/Login" exact element={<Login />} />
            <Route path="/Register" exact element={<Register />} />
            <Route path="/Chat" exact element={<Chat />} />
          </Routes>
        </div>
      </Router>
    </WebSocketProvider>
  );
}

export default App;