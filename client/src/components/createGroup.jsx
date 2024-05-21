import React, { useState } from 'react';

const CreateGroup = ({ showModal, closeModal, addGroup }) => {
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
            <h1 className="modal-title fs-5" id="exampleModalLabel">Crear grupo</h1>
            <button type="button" className="btn-close" data-bs-dismiss="modal" aria-label="Close" onClick={closeModal}></button>
          </div>
          <div className="modal-body">
            <input 
              className="form-control" 
              type="text" 
              placeholder="Nombre de grupo" 
              aria-label="default input example" 
              style={{marginBottom:40}} 
              value={groupName} 
              onChange={handleInputChange} 
            />
            <input className="form-control" type="text" placeholder="Administrador" aria-label="default input example" />
          </div>
          <div className="modal-footer">
            <button type="button" className="btn btn-secondary" data-bs-dismiss="modal" onClick={closeModal}>Close</button>
            <button type="button" className="btn btn-primary" onClick={handleSaveClick}>Save changes</button>
          </div>
        </div>
      </div>
    </div>
  );
};

export default CreateGroup;
