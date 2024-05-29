import React, { useState } from 'react';
import "./GroupParticipants.css";
import Data from "../test.json";

import { useWebSocket } from '../components/WebSocketConnection';  
import ActiveUsers from '../components/activeUsers';

function GroupParticipants({ group, onUpdateGroup }) {
  const [participants, setParticipants] = useState(group.Participants);
  const [showActiveUsersModal, setShowActiveUsersModal] = useState(false);
  const [user, setUser] = useState("Gabriel"); 
  const [groups, setGroups] = useState(Data);
  const sendMessage = useWebSocket();

  const openActiveUsersModal = () => {
    setShowActiveUsersModal(true);
  };

  const closeActiveUsersModal = () => {
    setShowActiveUsersModal(false);
  };

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
      
      <div className='optionsContainer'>
      <button onClick={() => handleLeaveChat(group.GroupName)} className='leaveBtn'>Leave Chat</button>
      <button onClick={openActiveUsersModal} className='addParticipants'><i class="bi bi-person-plus"></i></button>
      </div>
      <ActiveUsers showModal={showActiveUsersModal} closeModal={closeActiveUsersModal} user={user} groups={groups}/>
    </div>

);
}

export default GroupParticipants;
