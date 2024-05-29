import React, { useState } from 'react';
import "./GroupParticipants.css";
import { useWebSocket } from '../components/WebSocketConnection';  

function GroupParticipants({ group, onUpdateGroup }) {
  const [participants, setParticipants] = useState(group.Participants);
  const sendMessage = useWebSocket();

  const handleKickParticipant = (groupName, userName) => {
    const updatedParticipants = participants.filter(participant => participant.User !== userName);
    setParticipants(updatedParticipants);
    onUpdateGroup(groupName, updatedParticipants);

    sendMessage({ type: 'kick_participant', group: groupName, user: userName });
  };

  const handleLeaveChat = (groupName) => {
    onUpdateGroup(groupName, participants, true);

    sendMessage({ type: 'leave_chat', group: groupName });
  };

  if (!participants || participants.length === 0) {
    return <div>No participants available</div>;
  }

  return (
    <div className='groupParticipants'>
      {participants.map((participant, index) => (
        <div key={index} className='participant'>
          <div className='UserHolder'>
            {participant.User}
            <button onClick={() => handleKickParticipant(group.GroupName, participant.User)} className='kickButton'>
              <i className="bi bi-box-arrow-right"></i>
            </button>
          </div>
        </div>
      ))}
      <button onClick={() => handleLeaveChat(group.GroupName)} className='leaveBtn'>Leave Chat</button>
    </div>
  );
}

export default GroupParticipants;
