#pragma once
#include <consensus/params.h>
#include <primitives/block.h>
#include <validation.h>

namespace PoA {

// Returns true if header.qc proves quorum agreement at height.
// Fill state with REJECT_INVALID / "bad-qc" on failure.
bool CheckQuorumCertificate(const CBlockHeader& header,
                            const Consensus::Params& params,
                            int height,
                            CValidationState& state);

} // namespace PoA
