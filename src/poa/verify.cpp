#include <poa/verify.h>
#include <util.h>
#include <validation.h>

namespace PoA {

// Placeholder check: accept any non-empty `qc` as valid.
bool CheckQuorumCertificate(const CBlockHeader& header,
                            const Consensus::Params& params,
                            int height,
                            CValidationState& state)
{
    if (header.qc.empty()) {
        return state.DoS(100, false, REJECT_INVALID, "bad-qc-empty", false, "missing quorum certificate");
    }
    
    // For now, just accept any non-empty `qc`
    return true;
}

} // namespace PoA
