import React, { useState } from 'react';

const CreateGroup = ({ showModal, closeModal, addGroup }) => {
  const [groupName, setGroupName] = useState("");
  const [administratorId, setAdministratorId] = useState(""); 

  const handleInputChange = (event) => {
    setGroupName(event.target.value);
  };

  const handleAdminChange = (event) => {
    setAdministratorId(event.target.value);
  };

  const handleSaveClick = async () => {
    const newGroup = {
        name: groupName,
        administrator_id: administratorId
    };

    try {
        const response = await fetch('http://localhost:3002/channels', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(newGroup)
        });
        if (!response.ok) {
            const errorData = await response.json(); 
            throw new Error(`Error: ${response.statusText} - ${errorData.message}`);
        }
        const createdGroup = await response.json();
        addGroup(createdGroup);
        closeModal();
    } catch (error) {
        console.error('Error creating group:', error);
        alert(`Error creating group: ${error.message}`); 
    }
};


  return (
    <div className={`modal fade ${showModal ? 'show' : ''}`} style={{ display: showModal ? 'block' : 'none' }} id="exampleModal" tabIndex="-1" aria-labelledby="exampleModalLabel" aria-hidden={!showModal}>
      <div className="modal-dialog modal-dialog-centered">
        <div className="modal-content" style={{ backgroundColor: '#9f9f9f' }}>
          <div className="modal-header">
            <h1 className="modal-title fs-5" id="exampleModalLabel">Crear grupo</h1>
            <button type="button" className="btn-close" data-bs-dismiss="modal" aria-label="Close" onClick={closeModal}></button>
          </div>
          <div className="modal-body">
            <input
              className="form-control"
              type="text"
              placeholder="Nombre de grupo"
              aria-label="default input example"
              value={groupName}
              onChange={handleInputChange}
              style={{ marginBottom: 4 }}
            />
            <input
              className="form-control"
              type="text"
              placeholder="Administrator ID"
              aria-label="default input example"
              value={administratorId}
              onChange={handleAdminChange}
              style={{ marginBottom: 4 }}
            />
          </div>
          <div className="modal-footer">
            <button type="button" className="btn btn-secondary" data-bs-dismiss="modal" onClick={closeModal}>Close</button>
            <button type="button" className="btn btn-primary" onClick={handleSaveClick}>Save</button>
          </div>
        </div>
      </div>
    </div>
  );
};

export default CreateGroup;
