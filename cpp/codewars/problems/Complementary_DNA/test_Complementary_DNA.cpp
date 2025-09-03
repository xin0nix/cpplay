#include <gtest/gtest.h>
#include <sstream>
#include <string>

// Declaration of the function to be tested
std::string DNAStrand(const std::string &dna) {
  std::stringstream ss;
  for (char letter : dna) {
    switch (letter) {
    case 'A':
      ss << 'T';
      break;
    case 'T':
      ss << 'A';
      break;
    case 'G':
      ss << 'C';
      break;
    default:
      ss << 'G';
      break;
    }
  }
  return ss.str();
}

struct DNAStrandTestParam {
  std::string input;
  std::string expected;
};

class DNAStrandTest : public ::testing::TestWithParam<DNAStrandTestParam> {};

TEST_P(DNAStrandTest, ReturnsComplementaryStrand) {
  const auto &param = GetParam();
  EXPECT_EQ(DNAStrand(param.input), param.expected);
}

INSTANTIATE_TEST_SUITE_P(
    DNAStrandTests, DNAStrandTest,
    ::testing::Values(
        DNAStrandTestParam{"A", "T"}, DNAStrandTestParam{"T", "A"},
        DNAStrandTestParam{"C", "G"}, DNAStrandTestParam{"G", "C"},
        DNAStrandTestParam{"ATTGC", "TAACG"},
        DNAStrandTestParam{"GTAT", "CATA"},
        DNAStrandTestParam{"ACGTACGT", "TGCATGCA"},
        DNAStrandTestParam{"AAAA", "TTTT"}, DNAStrandTestParam{"CCCC", "GGGG"},
        DNAStrandTestParam{"GGGG", "CCCC"}, DNAStrandTestParam{"TTTT", "AAAA"},
        DNAStrandTestParam{std::string(1000, 'A'), std::string(1000, 'T')},
        // Empty string case (optional, depending on spec)
        DNAStrandTestParam{"", ""}));
