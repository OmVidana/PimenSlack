import React, { useState } from 'react';
import GroupParticipants from './GroupParticipants';
import "./chatCard.css";
function ChatCard({ group, onSendMessage, onKickParticipant, onLeaveChat }) {
  const [currentMessage, setCurrentMessage] = useState("");
  const [showParticipants, setShowParticipants] = useState(false);
  const [currentGroup, setCurrentGroup] = useState(group);

  const handleInputChange = (event) => {
    setCurrentMessage(event.target.value);
  };

  const handleSendClick = () => {
    if (currentMessage.trim() !== "") {
      const newMessage = {
        text: currentMessage,
        sender: 'user',
        time: new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })
      };
      onSendMessage(group.GroupName, newMessage);
      setCurrentMessage("");
    }
  };

  const handleUpdateGroup = (groupName, updatedParticipants, userLeft) => {
    if (userLeft) {
    } else {
      setCurrentGroup(prevGroup => ({
        ...prevGroup,
        Participants: updatedParticipants
      }));
    }
  };
  

  const handleParticipantsClick = () => {
    setShowParticipants(!showParticipants);
  };


  return (
    <div className='chat'>
      <div className='nameHeader'>
        <p className='userNameText'>{group.GroupName}</p>
        <button className='optionsBtn' onClick={handleParticipantsClick}>
          <i className="bi bi-three-dots" style={{ fontSize: 60, color: "#1f1f1f" }}></i>
        </button>
      </div>

      <div className='chatHolder'>
        <div className='messageHolder'>
          {group.messages.map((message, index) => (
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

          
{showParticipants && currentGroup.Participants && (
        <div className='participantsHolder'>
          <GroupParticipants group={currentGroup} onUpdateGroup={handleUpdateGroup} />
        </div>
      )}
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
