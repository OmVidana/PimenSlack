import React, { useState } from 'react';
import "../styles/Chat.css";
import Data from "../test.json";
import 'bootstrap-icons/font/bootstrap-icons.css';
import 'bootstrap/dist/css/bootstrap.css';
import CreateGroup from '../components/createGroup';
import ChatCard from '../components/chatCard';
import Notifications from '../components/notifications';
import { useWebSocket } from '../components/WebSocketConnection';

function Chat() {
  const [groups, setGroups] = useState(Data);
  const [selectedGroupName, setSelectedGroupName] = useState(null);
  const [showGroupModal, setShowGroupModal] = useState(false);
  const [showNotificationsModal, setShowNotificationsModal] = useState(false);
  const [user, setUser] = useState("Gabriel"); 
  const sendMessage = useWebSocket();

  const openGroupModal = () => {
    setShowGroupModal(true);
  };

  const closeGroupModal = () => {
    setShowGroupModal(false);
  };

  const openNotificationsModal = () => {
    setShowNotificationsModal(true);
  };

  const closeNotificationsModal = () => {
    setShowNotificationsModal(false);
  };

  const addGroup = (groupName) => {
    if (groupName.trim() !== "") {
      const newGroup = {
        GroupName: groupName,
        Message: "Nuevo grupo creado",
        Hour: new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' }),
        messages: [],
        Participants: [{ User: user, State: 0 }] // State 0 means admin
      };
      setGroups([...groups, newGroup]);
      sendMessage({ type: 'new_group', group: newGroup });
    }
  };

  const selectGroup = (groupName) => {
    const group = groups.find(group => group.GroupName === groupName);
    const participant = group.Participants.find(participant => participant.User === user);

    if (participant && participant.State === 1) { // State 1 means access granted
      setSelectedGroupName(groupName);
    } else {
      alert("You don't have access to this group yet. Please request access.");
    }
  };

  const requestAccess = (groupName) => {
    sendMessage({ type: 'request_access', user, group: groupName });
    alert("Access request sent to the admin.");
  };

  const handleSendMessage = (groupName, newMessage) => {
    setGroups(groups.map(group => 
      group.GroupName === groupName 
        ? { ...group, messages: [...group.messages, newMessage], Message: newMessage.text, Hour: newMessage.time } 
        : group
    ));
    sendMessage({ type: 'message', group: groupName, message: newMessage });
  };

  const updateGroups = (updatedGroups) => {
    setGroups(updatedGroups);
  };

  const selectedGroup = groups.find(group => group.GroupName === selectedGroupName);

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
          <button className="notificationsBtn" onClick={openNotificationsModal}>
            <i className="bi bi-bell-fill" style={{ fontSize: 55, color: "#8a8a8a" }} width="200px"></i>
          </button>
        </div>

        {/* Chat Menu */}
        <div className='chatMenu'>
          <div className='chatCard'>
            <i className="bi bi-person-fill" style={{ fontSize: 80, color: "#8a8a8a", paddingLeft: 10, display: "flex", paddingTop: 20, position: "relative" }}></i>
            <div className='usernameTag'>Usuario activo</div>
          </div>
          {groups.map((group, key) => (
            <div className='chatCard' key={key}>
              <button className='messageSelectionBtn' key={key}  onClick={() => selectGroup(group.GroupName)} disabled={!group.Participants.some(p => p.User === user && p.State === 1)}>
                  <p className="userName">
                    {group.GroupName}
                  </p>
                  <div className='messagePreview'>
                    {group.Message}
                  </div>
              </button>
              {!group.Participants.some(p => p.User === user && p.State === 1) && (
                <button className='requestAccessBtn' onClick={() => requestAccess(group.GroupName)}><i class="bi bi-send-plus-fill" style={{marginRight:20}}></i></button>
              )}
            </div>
          ))}
        </div>

        {/* Chat */}
        {selectedGroup && (
          <ChatCard group={selectedGroup} onSendMessage={handleSendMessage} />
        )}
      </header>
      <CreateGroup showModal={showGroupModal} closeModal={closeGroupModal} addGroup={addGroup} />
      <Notifications showModal={showNotificationsModal} closeModal={closeNotificationsModal} user={user} groups={groups} updateGroups={updateGroups}/>
    </div>
  );
}

export default Chat;
