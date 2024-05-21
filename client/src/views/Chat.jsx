import React, { useState } from 'react';
import "../styles/Chat.css";
import Data from "../test.json";
import 'bootstrap-icons/font/bootstrap-icons.css';
import 'bootstrap/dist/css/bootstrap.css';
import CreateGroup from '../components/createGroup';
import ChatCard from '../components/chatCard';

function Chat() {
  const [groups, setGroups] = useState(Data);
  const [selectedGroup, setSelectedGroup] = useState(null);
  const [showModal, setShowModal] = useState(false);

  const openGroupModal = () => {
    setShowModal(true);
  };

  const closeGroupModal = () => {
    setShowModal(false);
  };

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

  const selectGroup = (group) => {
    setSelectedGroup(group);
  };

  const handleSendMessage = (groupName, message) => {
    setGroups(groups.map(group =>
      group.GroupName === groupName ? { ...group, messages: [...group.messages, message] } : group
    ));
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
          {groups.map((group, key) => (
            <button className='messageSelectionBtn' key={key} onClick={() => selectGroup(group)}>
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
          <ChatCard group={selectedGroup} messages={selectedGroup.messages} onSendMessage={handleSendMessage} />
        )}
      </header>
      <CreateGroup showModal={showModal} closeModal={closeGroupModal} addGroup={addGroup} />
    </div>
  );
}

export default Chat;
