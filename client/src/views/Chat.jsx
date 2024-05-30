import React, { useState, useEffect } from 'react';
import { useLocation } from 'react-router-dom';
import "../styles/Chat.css";
import 'bootstrap-icons/font/bootstrap-icons.css';
import 'bootstrap/dist/css/bootstrap.css';
import CreateGroup from '../components/createGroup';
import ChatCard from '../components/chatCard';
import Notifications from '../components/notifications';
import { useWebSocket } from '../components/WebSocketConnection';

function Chat() {
  const location = useLocation();
  const user = location.state.user; 
  const [groups, setGroups] = useState([]);
  const [selectedGroupName, setSelectedGroupName] = useState(null);
  const [showGroupModal, setShowGroupModal] = useState(false);
  const [showNotificationsModal, setShowNotificationsModal] = useState(false);
  const { sendMessage, subscribe, unsubscribe } = useWebSocket();

  useEffect(() => {
    const fetchGroups = async () => {
      try {
        const response = await fetch('http://localhost:3002/channels');
        const data = await response.json();
        const formattedData = data.map(group => ({
          ...group,
          Participants: group.Participants || [],
          messages: [] 
        }));
        setGroups(formattedData);
      } catch (error) {
        console.error('Error fetching groups:', error);
      }
    };

    fetchGroups();
  }, []);

  useEffect(() => {
    const handleServerMessage = (message) => {
      if (message.type === 'update_groups') {
        setGroups(message.groups);
      }
    };

    subscribe(handleServerMessage);
    return () => {
      unsubscribe(handleServerMessage);
    };
  }, [subscribe, unsubscribe]);

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

  const addGroup = async (groupName) => {
    try {
      const response = await fetch('http://localhost:3002/channels', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify({ name: groupName, administrator_id: user.userId })
      });
      if (!response.ok) {
        throw new Error(`Error: ${response.statusText}`);
      }
      const newGroup = await response.json();
      setGroups([...groups, newGroup]);
      sendMessage({ type: 'new_group', group: newGroup });
    } catch (error) {
      console.error('Error creating group:', error);
    }
  };

  const selectGroup = (groupName) => {
    const group = groups.find(group => group.name === groupName);
    if (!group) return;

    setSelectedGroupName(groupName);

    fetchMessages(group.ID);
  };

  const fetchMessages = async (channelId) => {
    try {
      const response = await fetch(`http://localhost:3002/channels/${channelId}/messages`);
      const messages = await response.json();
      setGroups(groups => groups.map(group => group.ID === channelId ? { ...group, messages } : group));
    } catch (error) {
      console.error('Error fetching messages:', error);
    }
  };

  const handleSendMessage = async (groupName, newMessage) => {
    const group = groups.find(group => group.name === groupName);
    if (!group) return;

    try {
      const response = await fetch('http://localhost:3002/messages', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify({ msg: newMessage.text, user_id: user.userId, channel_id: group.ID })
      });
      if (!response.ok) {
        throw new Error(`Error: ${response.statusText}`);
      }
      const savedMessage = await response.json();
      setGroups(groups.map(g => g.name === groupName ? { ...g, messages: [...g.messages, savedMessage] } : g));
    } catch (error) {
      console.error('Error sending message:', error);
    }

    sendMessage({ type: 'message', group: groupName, message: newMessage });
  };

  const updateGroups = (updatedGroups) => {
    setGroups(updatedGroups);
  };

  const selectedGroup = groups.find(group => group.name === selectedGroupName);

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
              <button className='messageSelectionBtn' onClick={() => selectGroup(group.name)}>
                <p className="userName">
                  {group.name}
                </p>
                <div className='messagePreview'>
                  {group.Message}
                </div>
              </button>
            </div>
          ))}
        </div>

        {/* Chat */}
        {selectedGroup && (
          <ChatCard group={selectedGroup} onSendMessage={handleSendMessage} userId={user.userId} />
        )}
      </header>
      <CreateGroup showModal={showGroupModal} closeModal={closeGroupModal} addGroup={addGroup} />
      <Notifications showModal={showNotificationsModal} closeModal={closeNotificationsModal} user={user} groups={groups} updateGroups={updateGroups} />
    </div>
  );
}

export default Chat;
