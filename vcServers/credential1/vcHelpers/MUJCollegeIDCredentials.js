module.exports = {
  // VC type: MUJCollegeID
  // https://raw.githubusercontent.com/Sud0-AP/BUIDL-IRL-PolygonID/master/manipal-university-college-id.jsonld
  MUJCollegeID: (credentialSubject) => ({
    id: 1,
    circuitId: "credentialAtomicQuerySigV2",
    query: {
      allowedIssuers: ["*"],
      type: "MUJCollegeID",
      context:
        "https://raw.githubusercontent.com/Sud0-AP/BUIDL-IRL-PolygonID/master/manipal-university-college-id.jsonld",
      credentialSubject,
    },
  }),
};
