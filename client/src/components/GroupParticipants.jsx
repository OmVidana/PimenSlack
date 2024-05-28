import React, { useState } from 'react';
import "./GroupParticipants.css";

function GroupParticipants({ group, onUpdateGroup }) {
  const [participants, setParticipants] = useState(group.Participants);

  const handleKickParticipant = (groupName, userName) => {
    const updatedParticipants = participants.filter(participant => participant.User !== userName);
    setParticipants(updatedParticipants);
    // Optionally call a function to update the group in the parent component or server
    onUpdateGroup(groupName, updatedParticipants);
  };

  const handleLeaveChat = (groupName) => {
    // Implement logic to handle leaving the chat
    console.log(`Leaving chat: ${groupName}`);
    // Call a function to handle the user leaving the chat in the parent component or server
    onUpdateGroup(groupName, participants, true); // Pass a flag or additional parameter if needed
  };

  console.log('Rendering GroupParticipants with group:', group);

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
