import React, { useState } from 'react';
import "../styles/Chat.css";
import Data from "../test.json";
import 'bootstrap-icons/font/bootstrap-icons.css';
import 'bootstrap/dist/css/bootstrap.css';
import CreateGroup from './createGroup';

function ChatCard({ user, message, hour }) {
  const [messages, setMessages] = useState([]);
  const [currentMessage, setCurrentMessage] = useState("");

  const handleInputChange = (event) => {
    setCurrentMessage(event.target.value);
  };

  const handleSendClick = () => {
    if (currentMessage.trim() !== "") {
      setMessages([...messages, { text: currentMessage, sender: 'user' }]);
      setCurrentMessage("");
    }
  };

  return (
    <div className='chat'>
      <div className='nameHeader'>
        <p className='userNameText'>{user}</p>
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
  );
}

export default ChatCard;