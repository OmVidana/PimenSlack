import React from 'react';
import { useState, useEffect } from 'react';
import "../styles/Chat.css";
import { Link } from 'react-router-dom';
import Data from "../test.json"
import 'bootstrap-icons/font/bootstrap-icons.css';
import 'bootstrap/dist/css/bootstrap.css';




function Chat(){
    const [userName, setUserName] = useState("")
    const [messages, setMessages] = useState([]);
    const [currentMessage, setCurrentMessage] = useState("");

    const handleInputChange = (event) => {
        setCurrentMessage(event.target.value);
      };

      const handleSendClick = () => {
        if (currentMessage.trim() !== "") {
          setMessages([...messages, { text: currentMessage, sender: 'user' }]);
          setCurrentMessage("");
        }
      };


    return(
<div className='Login'>
    <header className="App-header">
        {/*BUTTONS*/}
    <div className='buttons'>
{/*Personal*/}
    <button className="personalBtn">
    <i class="bi bi-person-fill" style={{fontSize:55, color: "#8a8a8a"}} ></i>
        </button>
{/*Groups*/}
<button className="groupsBtn">
    <i class="bi bi-people-fill" style={{fontSize:55, color: "#8a8a8a"}}  width="200px" ></i>
        </button>

{/*Notifications*/}
<button className="notificationsBtn">
    <i class="bi bi-bell-fill" style={{fontSize:55, color: "#8a8a8a"}}  width="200px" ></i>
        </button>  
        </div>


        {/*Chat Menu*/}
        <div className='chatMenu'>
        <div className='chatCard'></div>
        {Data.map((data, key) => {
            return(
                <button className='messageSelectionBtn'>
            <div className='chatCard' key={key}>   
    <p className="userName">
        {data.User}
<p>
        {data.Hour}
    </p>
</p>
    <div className='messagePreview'>
        {data.Message}
    </div>
        {/* <span class="position-absolute top-0 start-100 translate-middle p-2 bg-danger border border-light rounded-circle"/> */}
            </div>
            </button>
            )
        })}
        </div>
        {/*UserChatCard*/}



      {/*Chat */}
     {/* Chat */}
     <div className='chat'>
          <div className='nameHeader'>
            <p className='userNameText'>Grupo de prueba</p>
            <button className='optionsBtn'>
              <i className="bi bi-three-dots" style={{ fontSize: 60, color: "#1f1f1f" }}></i>
            </button>
          </div>

          <div className='chatHolder'>
            <div className='messageHolder'>
              {messages.map((message, index) => (
                <div key={index} className={`message ${message.sender === 'user' ? 'userMsg' : 'otherMsg'}`}>
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
      </header>
    </div>
  );
}

export default Chat;