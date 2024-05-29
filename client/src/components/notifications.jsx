import React, { useState } from 'react';
import "./notifications.css";
import 'bootstrap-icons/font/bootstrap-icons.css';
import 'bootstrap/dist/css/bootstrap.css';

const Notifications = ({ showModal, closeModal, addGroup, user, groups, updateGroups }) => {
  const [groupName, setGroupName] = useState("");

  const handleInputChange = (event) => {
    setGroupName(event.target.value);
  };

  const acceptInvitation = (groupName) => {
    const updatedGroups = groups.map(group => {
      if (group.GroupName === groupName) {
        group.Participants.forEach(participant => {
          if (participant.User === user) {
            participant.State = 1; // Change state to accepted
          }
        });
      }
      return group;
    });
    // Update JSON and setGroups
    updateGroups(updatedGroups);
    closeModal();
  };

  const rejectInvitation = (groupName) => {
    // For rejecting, no need to change state, just close modal
    closeModal();
  };

  return (
    <div className={`modal fade ${showModal ? 'show' : ''}`} style={{ display: showModal ? 'block' : 'none' }} id="exampleModal" tabindex="-1" aria-labelledby="exampleModalLabel" aria-hidden={!showModal}>
      <div className="modal-dialog modal-dialog-centered">
        <div className="modal-content" style={{ backgroundColor: '#9f9f9f' }}> 
          <div className="modal-header">
            <h1 className="modal-title fs-5" id="exampleModalLabel">Notificaciones</h1>
            <button type="button" className="btn-close" data-bs-dismiss="modal" aria-label="Close" onClick={closeModal}></button>
          </div>
          <div className="modal-body">
            {groups.map(group => (
              group.Participants.find(participant => participant.User === user && participant.State === 0) ? (
                <div key={group.GroupName} className='notification'>
                  <div className='notificationName'>{group.GroupName}</div>
                  <button type='button' onClick={() => rejectInvitation(group.GroupName)} className="btn btn-danger">Rechazar</button>
                  <button type='button' onClick={() => acceptInvitation(group.GroupName)} className="btn btn-success">Aceptar</button>
                </div>
              ) : null
            ))}
          </div>
          <div className="modal-footer">
            <button type="button" className="btn btn-secondary" data-bs-dismiss="modal" onClick={closeModal}>Cerrar</button>
          </div>
        </div>
      </div>
    </div>
  );
};

export default Notifications;
