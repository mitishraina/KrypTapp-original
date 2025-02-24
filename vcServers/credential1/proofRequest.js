const { MUJCollegeID } = require("./vcHelpers/MUJCollegeIDCredentials");

// design your own customised authentication requirement here using Query Language
// https://0xpolygonid.github.io/tutorials/verifier/verification-library/zk-query-language/

const humanReadableAuthReason =
  "Must be enrolled to with the college to access this page";

const credentialSubject = {
  enrolled: {
    $eq: true,
  },
};

const proofRequest = MUJCollegeID(credentialSubject);

module.exports = {
  humanReadableAuthReason,
  proofRequest,
};
