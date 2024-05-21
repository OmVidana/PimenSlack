import React, { useState } from 'react';
import "../styles/Chat.css";
import Data from "../test.json";
import 'bootstrap-icons/font/bootstrap-icons.css';
import 'bootstrap/dist/css/bootstrap.css';
import CreateGroup from '../components/createGroup';

function Chat() {
  
  const [userName, setUserName] = useState("");
  const [messages, setMessages] = useState([]);
  const [currentMessage, setCurrentMessage] = useState("");
  const [showModal, setShowModal] = useState(false);
  const [groups, setGroups] = useState(Data);

  const handleInputChange = (event) => {
    setCurrentMessage(event.target.value);
  };

  const handleSendClick = () => {
    if (currentMessage.trim() !== "") {
      setMessages([...messages, { text: currentMessage, sender: 'user' }]);
      setCurrentMessage("");
    }
  };

  const openGroupModal = () => {
    setShowModal(true);
  };

  const closeGroupModal = () => {
    setShowModal(false);
  };

  const addGroup = (groupName) => {
    if (groupName.trim() !== "") {
      const newGroup = {
        User: groupName,
        Message: "Nuevo grupo creado",
        Hour: new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })
      };
      setGroups([...groups, newGroup]);
    }
  };

  return (
    <div className='Login'>
      <header className="App-header">
        {/* BUTTONS */}
        <div className='buttons'>
          {/* Personal */}
          <button className="personalBtn">
            <i className="bi bi-person-fill" style={{ fontSize: 55, color: "#8a8a8a" }}></i>
          </button>
          {/* Groups */}
          <button className="groupsBtn" onClick={openGroupModal}>
            <i className="bi bi-people-fill" style={{ fontSize: 55, color: "#8a8a8a" }} width="200px"></i>
          </button>
          {/* Notifications */}
          <button className="notificationsBtn">
            <i className="bi bi-bell-fill" style={{ fontSize: 55, color: "#8a8a8a" }} width="200px"></i>
          </button>
        </div>

        {/* Chat Menu */}
        <div className='chatMenu'>
          <div className='chatCard'>
            <i className="bi bi-person-fill" style={{ fontSize: 80, color: "#8a8a8a", paddingLeft: 10, display: "flex", paddingTop: 20, position: "relative" }}></i>
            <div className='usernameTag'>Usuario activo</div>
          </div>
          {groups.map((data, key) => (
            <button className='messageSelectionBtn' key={key}>
              <div className='chatCard'>
                <p className="userName">
                  {data.User}
                  <p>{data.Hour}</p>
                </p>
                <div className='messagePreview'>
                  {data.Message}
                </div>
              </div>
            </button>
          ))}
        </div>

        {/* Chat */}
        <div className='chat'>
          <div className='nameHeader'>
            <p className='userNameText'>Grupo de prueba</p>
            <button className='optionsBtn'>
              <i className="bi bi-three-dots" style={{ fontSize: 60, color: "#1f1f1f" }}></i>
            </button>
          </div>

          <div className='chatHolder'>
            <div className='messageHolder'>
              {messages.map((message, index) => (
                <div key={index} className={`message ${message.sender === 'user' ? 'userMsg' : 'otherMsg'}`}>
                  {message.text}
                </div>
              ))}
            </div>

            <div className='inputGroup'>
              <input
                type="text"
                className='textPlaceholder'
                value={currentMessage}
                onChange={handleInputChange}
                placeholder='Escribe un mensaje'
                style={{ fontSize: 35, backgroundColor: "#bfbfbf" }}
              />
              <button className='sendBtn' onClick={handleSendClick}>
                <i className="bi bi-send" style={{ fontSize: 35, color: "#b3b3b3" }}></i>
              </button>
            </div>
          </div>
        </div>
      </header>
      <CreateGroup showModal={showModal} closeModal={closeGroupModal} addGroup={addGroup} />
    </div>
  );
}

export default Chat;
