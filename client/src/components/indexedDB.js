import { openDB } from 'idb';

const DB_NAME = 'pimenslack_db';
const DB_VERSION = 1;
const STORE_NAMES = ['users', 'channels', 'messages', 'channels_users', 'channel_messages'];

const initDB = async () => {
  const db = await openDB(DB_NAME, DB_VERSION, {
    upgrade(db) {
      STORE_NAMES.forEach(storeName => {
        if (!db.objectStoreNames.contains(storeName)) {
          db.createObjectStore(storeName, { keyPath: 'id' });
        }
      });
    },
  });
  return db;
};

export const addItem = async (storeName, item) => {
  const db = await initDB();
  const tx = db.transaction(storeName, 'readwrite');
  const store = tx.objectStore(storeName);
  await store.add(item);
  await tx.done;
};

export const updateItem = async (storeName, item) => {
  const db = await initDB();
  const tx = db.transaction(storeName, 'readwrite');
  const store = tx.objectStore(storeName);
  await store.put(item);
  await tx.done;
};

export const getAllItems = async (storeName) => {
  const db = await initDB();
  const tx = db.transaction(storeName, 'readonly');
  const store = tx.objectStore(storeName);
  const items = await store.getAll();
  await tx.done;
  return items;
};

export const deleteItem = async (storeName, id) => {
  const db = await initDB();
  const tx = db.transaction(storeName, 'readwrite');
  const store = tx.objectStore(storeName);
  await store.delete(id);
  await tx.done;
};