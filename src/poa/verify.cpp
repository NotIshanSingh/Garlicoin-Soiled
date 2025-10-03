#include <poa/verify.h>
#include <util.h>

namespace PoA {

bool CheckQuorumCertificate(const CBlockHeader& header,
                            const Consensus::Params& params,
                            int height,
                            CValidationState& state)
{
    // BOOTSTRAP: accept any non-empty qc (easy bring-up)
    // Replace with real signature aggregation check below.
    if (params.poa.vValidatorPubKeys.empty()) {
        return state.DoS(100, false, REJECT_INVALID, "bad-qc-config", false, "no validators configured");
    }
    if (header.qc.empty()) {
        return state.DoS(100, false, REJECT_INVALID, "bad-qc-empty", false, "missing quorum certificate");
    }
    return true;
}

} // namespace PoA
