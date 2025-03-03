import Hyperswarm from 'hyperswarm';
import Hypercore from 'hypercore';


// Create a Hypercore storage for user identities
const identityCore = new Hypercore('./krytapp-identity');
await identityCore.ready();



const swarm = new Hyperswarm();
swarm.join(identityCore.discoveryKey);
swarm.on('connection', (conn) => {
  console.log('Connected to a peer for identity verification');
  identityCore.replicate(conn);
});


async function storeIdentity(userId, encryptedIdentityData) {
  const identityRecord = {
    userId,
    encryptedData: encryptedIdentityData,
    timestamp: Date.now(),
  };

  await identityCore.append(JSON.stringify(identityRecord));
  console.log('Identity stored for user:', userId);
}

// Function to verify identity by fetching data from peers
async function verifyIdentity(userId) {
  for await (const record of identityCore.createReadStream()) {
    const data = JSON.parse(record.toString());
    if (data.userId === userId) {
      console.log('Identity verified for user:', userId);
      return data.encryptedData;
    }
  }
  console.log('Identity not found for user:', userId);
  return null;
}

export { storeIdentity, verifyIdentity };
