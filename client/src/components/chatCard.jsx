import React, { useState, useEffect } from 'react';
import GroupParticipants from './GroupParticipants';
import "./chatCard.css";

function ChatCard({ group, onSendMessage, userId }) {
  const [currentMessage, setCurrentMessage] = useState("");
  const [messages, setMessages] = useState([]);
  const [currentGroup, setCurrentGroup] = useState(group);

  useEffect(() => {
    const fetchMessages = async () => {
      try {
        const response = await fetch(`http://localhost:3002/channels/${group.ID}/messages`);
        const data = await response.json();

        // Asegúrate de que los mensajes recibidos del backend tengan el formato correcto
        const formattedMessages = data.map(msg => ({
          text: msg.msg,  // Ajusta esto según la estructura de tu base de datos
          sender: msg.user_id === userId ? 'user' : 'other',  // Diferenciar entre mensajes del usuario actual y otros
          time: new Date(msg.sent_date).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })  // Ajusta según tu estructura de datos
        }));

        setMessages(formattedMessages);
      } catch (error) {
        console.error('Error fetching messages:', error);
      }
    };

    fetchMessages();
  }, [group.ID, userId]);

  const handleInputChange = (event) => {
    setCurrentMessage(event.target.value);
  };

  const handleSendClick = async () => {
    if (currentMessage.trim() !== "") {
      const newMessage = {
        text: currentMessage,
        sender: 'user',
        time: new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })
      };
      await onSendMessage(group.name, newMessage);
      setMessages([...messages, newMessage]);
      setCurrentMessage("");
    }
  };

  return (
    <div className='chat'>
      <div className='nameHeader'>
        <p className='userNameText'>{group.name}</p>
      </div>

      <div className='chatHolder'>
        <div className='messageHolder'>
          {messages.map((message, index) => (
            <div key={index} className={`message ${message.sender === 'user' ? 'userMsg' : 'otherMsg'}`}>
              <div style={{ fontWeight: "bold", color: "#1f1f1f" }}>
                {message.sender}
              </div>
              {message.text}
              <div style={{ fontSize: "small", color: "#1f1f1f" }}>
                {message.time}
              </div>
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
