import React, { useState } from 'react';
import "../styles/Chat.css";
import Data from "../test.json";
import 'bootstrap-icons/font/bootstrap-icons.css';
import 'bootstrap/dist/css/bootstrap.css';
import CreateGroup from '../components/createGroup';
import ChatCard from '../components/chatCard';

function Chat() {
  const [groups, setGroups] = useState(Data);
  const [selectedGroupName, setSelectedGroupName] = useState(null);
  const [showGroupModal, setShowGroupModal] = useState(false);
  const [showNotificationsModal, setShowNotificationsModal] = useState(false)

  const openGroupModal = () => {
    setShowGroupModal(true);
  };

  const closeGroupModal = () => {
    setShowGroupModal(false);
  };

  const openNotificationsModal = () => {
    setShowNotificationsModal(true);
  }

  const closeNotificationsModal = () => {
    setShowNotificationsModal(false);
  }

  const addGroup = (groupName) => {
    if (groupName.trim() !== "") {
      const newGroup = {
        GroupName: groupName,
        Message: "Nuevo grupo creado",
        Hour: new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' }),
        messages: []
      };
      setGroups([...groups, newGroup]);
    }
  };

  const selectGroup = (groupName) => {
    setSelectedGroupName(groupName);
  };

  const handleSendMessage = (groupName, newMessage) => {
    setGroups(groups.map(group => 
      group.GroupName === groupName 
        ? { ...group, messages: [...group.messages, newMessage], Message: newMessage.text, Hour: newMessage.time } 
        : group
    ));
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
            <button className='messageSelectionBtn' key={key} onClick={() => selectGroup(group.GroupName)}>
              <div className='chatCard'>
                <p className="userName">
                  {group.GroupName}
                  <p>{group.Hour}</p>
                </p>
                <div className='messagePreview'>
                  {group.Message}
                </div>
              </div>
            </button>
          ))}
        </div>

        {/* Chat */}
        {selectedGroup && (
          <ChatCard group={selectedGroup} onSendMessage={handleSendMessage} />
        )}
      </header>
      <CreateGroup showModal={showGroupModal} closeModal={closeGroupModal} addGroup={addGroup} />
    </div>
  );
}

export default Chat;
