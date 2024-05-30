import React, { useState } from 'react';
import "./notifications.css";
import 'bootstrap-icons/font/bootstrap-icons.css';
import 'bootstrap/dist/css/bootstrap.css';

const Notifications = ({ showModal, closeModal, user, groups, updateGroups }) => {
  const acceptInvitation = async (groupName, groupId) => {
    const userId = 1; // Aquí deberías obtener el ID del usuario actual
    try {
        const response = await fetch('http://localhost:3002/accept-request', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ user_id: userId, channel_id: groupId })
        });
        if (!response.ok) {
            throw new Error(`Error: ${response.statusText}`);
        }
        const updatedGroups = groups.map(group => {
            if (group.name === groupName) {
                group.Participants.forEach(participant => {
                    if (participant.User === user) {
                        participant.State = 1; // Cambiar estado a aceptado
                    }
                });
            }
            return group;
        });
        updateGroups(updatedGroups);
        closeModal();
    } catch (error) {
        console.error('Error accepting invitation:', error);
    }
};

  const rejectInvitation = (groupName) => {
    // Para rechazar, simplemente cierra el modal
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
              group.Participants.find(participant => participant.User === user && participant.State === 2) ? (
                <div key={group.name} className='notification'>
                  <div className='notificationName'>{group.name}</div>
                  <button type='button' onClick={() => rejectInvitation(group.name)} className="btn btn-danger">Rechazar</button>
                  <button type='button' onClick={() => acceptInvitation(group.name, group.ID)} className="btn btn-success">Aceptar</button>
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
