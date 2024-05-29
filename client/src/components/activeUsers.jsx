import React, { useState } from 'react';

const ActiveUsers = ({ showModal, closeModal,user, groups, addGroup }) => {
  const [groupName, setGroupName] = useState("");

  const handleInputChange = (event) => {
    setGroupName(event.target.value);
  };

  const handleSaveClick = () => {
    addGroup(groupName);
    closeModal();
  };

  return (
    <div className={`modal fade ${showModal ? 'show' : ''}`} style={{ display: showModal ? 'block' : 'none' }} id="exampleModal" tabindex="-1" aria-labelledby="exampleModalLabel" aria-hidden={!showModal}>
      <div className="modal-dialog modal-dialog-centered">
        <div className="modal-content" style={{ backgroundColor: '#9f9f9f' }}> 
          <div className="modal-header">
            <h1 className="modal-title fs-5" id="exampleModalLabel">Usuarios activos</h1>
            <button type="button" className="btn-close" data-bs-dismiss="modal" aria-label="Close" onClick={closeModal}></button>
          </div>
          <div className="modal-body">
          {groups.map(group => (
              group.Participants.find(participant => participant.User === user && participant.State === 0) ? (
                <div key={group.GroupName} className='notification'>
                  <div className='notificationName'>{group.GroupName}</div>
                  <button type='button'className="btn btn-success">Invitar</button>
                </div>
              ) : null
            ))}
          </div>
          <div className="modal-footer">
            <button type="button" className="btn btn-secondary" data-bs-dismiss="modal" onClick={closeModal}>Close</button>
          </div>
        </div>
      </div>
    </div>
  );
};

export default ActiveUsers;
