// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CONSENSUS_PARAMS_H
#define BITCOIN_CONSENSUS_PARAMS_H

#include <uint256.h>
#include <limits>
#include <map>
#include <string>
#include <vector>   // NEW: for PoA vectors

namespace Consensus {

enum DeploymentPos
{
    DEPLOYMENT_TESTDUMMY,
    DEPLOYMENT_CSV,    // Deployment of BIP68, BIP112, and BIP113.
    DEPLOYMENT_SEGWIT, // Deployment of BIP141, BIP143, and BIP147.
    // NOTE: Also add new deployments to VersionBitsDeploymentInfo in versionbits.cpp
    MAX_VERSION_BITS_DEPLOYMENTS
};

/**
 * Struct for each individual consensus rule change using BIP9.
 */
struct BIP9Deployment {
    /** Bit position to select the particular bit in nVersion. */
    int bit;
    /** Start MedianTime for version bits miner confirmation. Can be a date in the past */
    int64_t nStartTime;
    /** Timeout/expiry MedianTime for the deployment attempt. */
    int64_t nTimeout;

    /** Constant for nTimeout very far in the future. */
    static constexpr int64_t NO_TIMEOUT = std::numeric_limits<int64_t>::max();

    /** Special value for nStartTime indicating that the deployment is always active.
     *  This is useful for testing, as it means tests don't need to deal with the activation
     *  process (which takes at least 3 BIP9 intervals). Only tests that specifically test the
     *  behaviour during activation cannot use this. */
    static constexpr int64_t ALWAYS_ACTIVE = -1;
};

/**
 * Minimal Proof-of-Agreement (SCP-like) configuration.
 *
 * - vValidatorPubKeys: Ed25519 public keys (32-byte) serialized as raw bytes.
 * - nActivationHeight: hard-fork height where PoA replaces PoW checks.
 * - nSlotDurationSeconds: target "ledger close" period for one slot/height.
 * - nQuorumThresholdPercent: % of total validator weight required (e.g., 67).
 * - vValidatorWeights: optional per-validator weights (same order as pubkeys).
 *
 * NOTE: Keep this intentionally simple. More advanced quorum-sets (nested
 * sets / innerSets) can be layered later without breaking this surface.
 */
struct PoAConfig {
    int nActivationHeight{std::numeric_limits<int>::max()}; // default: never activate
    uint32_t nSlotDurationSeconds{10};                      // default: 10s slots
    uint32_t nQuorumThresholdPercent{67};                   // default: 67%
    std::vector<std::vector<unsigned char>> vValidatorPubKeys;
    std::vector<uint32_t> vValidatorWeights;                // optional; empty = all weight 1

    bool IsActiveAt(int height) const {
        return height >= nActivationHeight;
    }
};

/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    int nSubsidyHalvingInterval;
    /** Block height at which BIP16 becomes active */
    int BIP16Height;
    /** Block height and hash at which BIP34 becomes active */
    int BIP34Height;
    uint256 BIP34Hash;
    /** Block height at which BIP65 becomes active */
    int BIP65Height;
    /** Block height at which BIP66 becomes active */
    int BIP66Height;
    /**
     * Minimum blocks including miner confirmation of the total of 2016 blocks in a retargeting period,
     * (nPowTargetTimespan / nPowTargetSpacing) which is also used for BIP9 deployments.
     * Examples: 1916 for 95%, 1512 for testchains.
     */
    uint32_t nRuleChangeActivationThreshold;
    uint32_t nMinerConfirmationWindow;
    BIP9Deployment vDeployments[MAX_VERSION_BITS_DEPLOYMENTS];

    /** Proof of work parameters */
    int64_t DifficultyAdjustmentInterval() const { return nPowTargetTimespan / nPowTargetSpacing; }
    uint256 nMinimumChainWork;
    uint256 defaultAssumeValid;

    // ----- NEW: Proof-of-Agreement (SCP-like) parameters -----
    PoAConfig poa;

    // NOTE: The following PoW timing members are referenced by DifficultyAdjustmentInterval()
    // and exist in the original Params struct in your tree. They are *not* defined in this header
    // excerpt but must remain available in the translation unit:
    //   int64_t nPowTargetTimespan;
    //   int64_t nPowTargetSpacing;
    // Keep them as-is; PoW code will ignore them after PoA activation.
};

} // namespace Consensus

#endif // BITCOIN_CONSENSUS_PARAMS_H
