import React, { useState } from 'react';

function ChatCard({ group, messages, onSendMessage }) {
  const [currentMessage, setCurrentMessage] = useState("");

  const handleInputChange = (event) => {
    setCurrentMessage(event.target.value);
  };

  const handleSendClick = () => {
    if (currentMessage.trim() !== "") {
      onSendMessage(group.GroupName, { text: currentMessage, sender: 'user' });
      setCurrentMessage("");
    }
  };

  return (
    <div className='chat'>
      <div className='nameHeader'>
        <p className='userNameText'>{group.GroupName}</p>
        <button className='optionsBtn'>
          <i className="bi bi-three-dots" style={{ fontSize: 60, color: "#1f1f1f" }}></i>
        </button>
      </div>

      <div className='chatHolder'>
        <div className='messageHolder'>
          {messages.map((message, index) => (
            <div key={index} className={`message ${message.sender === 'user' ? 'userMsg' : 'otherMsg'}`}>
              <div style={{ fontWeight: "bold", color: "#1f1f1f" }}>
                {message.sender}
              </div>
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
