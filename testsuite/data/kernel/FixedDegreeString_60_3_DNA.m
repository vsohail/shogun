accuracy = 1e-09;
init_random = 42;
name = 'FixedDegreeString';
data_type = '';
km_train = [1, 0.0172413793, 0.0172413793, 0, 0.0689655172, 0, 0, 0, 0.0344827586, 0, 0.0344827586;0.0172413793, 1, 0, 0.0172413793, 0.0172413793, 0, 0, 0, 0.0344827586, 0, 0.0344827586;0.0172413793, 0, 1, 0.0344827586, 0, 0, 0, 0, 0, 0, 0;0, 0.0172413793, 0.0344827586, 1, 0, 0, 0, 0, 0, 0, 0.0517241379;0.0689655172, 0.0172413793, 0, 0, 1, 0.0172413793, 0.0172413793, 0, 0, 0, 0.0517241379;0, 0, 0, 0, 0.0172413793, 1, 0.0172413793, 0, 0, 0.0172413793, 0;0, 0, 0, 0, 0.0172413793, 0.0172413793, 1, 0, 0.0344827586, 0, 0.0172413793;0, 0, 0, 0, 0, 0, 0, 1, 0, 0.0172413793, 0.0172413793;0.0344827586, 0.0344827586, 0, 0, 0, 0, 0.0344827586, 0, 1, 0.0344827586, 0;0, 0, 0, 0, 0, 0.0172413793, 0, 0.0172413793, 0.0344827586, 1, 0;0.0344827586, 0.0344827586, 0, 0.0517241379, 0.0517241379, 0, 0.0172413793, 0.0172413793, 0, 0, 1];
data_train = {'TTGCCCCAGCGCCATGAGTTGAGTGTCATGACTAACTTGGCATACGAGGGACGTATGTAC', 'ACTAGAGAACTAAATGGGAGACGGAATGAATTTCCAGAAAGCGTGCTCATAGCGGAGTCC', 'CTGTAAAAGTCTATATATACCCGTGGAGCTCGACCCCATATAAGGCGCGGCTGCTTGATA', 'ACGTGTAAGTGAAACATGGAAGTGTTAAGCCCCTACGGGTTGACGTTGGAAAACCGGGCC', 'TATCGAACGCGCCTTGAATATCTCCGTTGTGTTCGGGTCACACTGTATGAGTAGTAGCCG', 'ATACGCCTTTGTTCCACTGGAACTCTAGGTAAATAAGGTTGTGCTGCGCAGTGATACCAC', 'GTTTCGTCTCCGGGCGTTAAGCACTTCCGAGACCGGCACGAAGTCCTTCTGTCTTCTCAC', 'TGGGACGTACAACGAATTCGCTGTAGGCCGGAGAAGCCCGCTCGCGGCCCAAGTCCAATG', 'ACTACGGCTAGAGTCGTAGTGAACAGAACCGGAAACAGATCCTGGTCTCTCCGAACGTGT', 'GAAGCACGCTGGTCGTGTCGATTTGAATCCGCGCGTACCTCCGCTTGGAGGCATCGACTC', 'GACTGTTTTATAAACGCACCTCACCGCCCCTATGATTGCGCATTATGACCTTAGTTAGTC'};
feature_class = 'string';
seqlen = 60;
data_test = {'CTATCAAAAGATGCCGCATCAGGCTGGCTCGCGAATCCGGAATGCTGAACTAATAGAGCA', 'GGGGGGGGGGAACTTGCATTATCCGGTACCCGCCCGGGACAACAGTATAGGTACACTTGG', 'TCATCAAGGCGTTGTTCCCACCAACGTGCCAATCTGAGCCGTTTAAACCGGTTATCCTAT', 'GTTGAACATCTGACCCGAGCTTAAGTCCACCCGCACTCTGCAGGGTGATGCGGACCCAAA', 'CTACCTAAATGACAATCGCGCCGAGTATACGGATTATGTATATGCATGCCTCATCCATAG', 'TATCGCGCATATACTATCCGCCGCATTCGGGCTAGTGTCCTCGATGCCGAGGATCCCGTA', 'CATCGCTGTAGTGATCACGTCCGCTTCATAAAATGTACTTGATCGGGGGACGCCACTCGG', 'ATCTAACCTAGATTCAGAAGTTGGTGCTAAGACCGTACAGGCAGGACCTTGTGTACGGAC', 'CCCGTGTTCCATAGCTCTCCGTGTTTCCATTGCATATCCTACTGTTAACCCTAGAACTAG', 'GAGTGAGGAAAACCCTGTGACTAACCCACGCGGGAGACGACAACTCCGGTTTTAGTGTAC', 'TCGAGTCAATGAATTACTGTCGGTCACTCCGAACGGTTCGAAACACGCGCAAAAGTCCTA', 'AGGGGACATTCACACGTCAAAATATGGCGCCCTCCCAAAGCTGAGGGGGAGCGATCGTTG', 'ATGGTAAGTCGCGTAGTCCATCCCCGTCGGCATGGATGTTTTATATCGATACTGACAGTG', 'ACTAGCTTCGGCCGATAACACCGTTATCCCCTCGATTCGGGGGAGCCTACAACTCGAGTC', 'TTGCGTACCCTCCGAAGCCAGTTAGTCTTACAATTAGGCGTAAACCCGTCCTTACTACCA', 'ACTTTAATGCACCATATTCGGACGGGCCCCGTGGGGGATACAATCTCCCGTCCTACCACA', 'TGTGGGTACGTTGAATCATAGGACAGCATCCAATCCTGCTGAGTCGTGAAGCTCCGGCAG'};
kernel_arg0_degree = 3;
km_test = [0, 0, 0.0172413793, 0.0517241379, 0, 0.0172413793, 0.0344827586, 0.0172413793, 0, 0.0517241379, 0, 0, 0.0172413793, 0.0172413793, 0.0517241379, 0.0344827586, 0.0172413793;0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0172413793, 0, 0.0172413793, 0.0172413793, 0.0517241379, 0, 0.0172413793, 0;0.0172413793, 0, 0, 0, 0.0517241379, 0.0172413793, 0.0172413793, 0.0344827586, 0, 0, 0.0344827586, 0.0172413793, 0, 0.0517241379, 0.0172413793, 0.0172413793, 0;0, 0, 0, 0.0172413793, 0.0344827586, 0, 0.0172413793, 0, 0, 0.0172413793, 0.0517241379, 0.0517241379, 0, 0, 0.0172413793, 0, 0.0344827586;0, 0.0862068966, 0.0344827586, 0, 0, 0.0862068966, 0.0344827586, 0, 0.0172413793, 0.0172413793, 0.0172413793, 0.0172413793, 0.0689655172, 0.0172413793, 0, 0.0344827586, 0.0172413793;0, 0, 0.0172413793, 0, 0.0172413793, 0.0344827586, 0.0689655172, 0.0172413793, 0, 0, 0.0344827586, 0.0172413793, 0.0172413793, 0, 0.0344827586, 0, 0.0172413793;0.0172413793, 0, 0, 0.0344827586, 0, 0, 0.0517241379, 0.0862068966, 0.0344827586, 0, 0.0517241379, 0.0172413793, 0, 0, 0, 0, 0.0344827586;0.0344827586, 0.0344827586, 0, 0, 0.0517241379, 0.0344827586, 0, 0, 0.0172413793, 0.0172413793, 0.0172413793, 0.0517241379, 0, 0.0344827586, 0.0344827586, 0.0517241379, 0;0, 0.0172413793, 0, 0.0172413793, 0.0344827586, 0, 0.0172413793, 0.0689655172, 0.0172413793, 0, 0.0172413793, 0.0517241379, 0, 0.0344827586, 0, 0.0344827586, 0.0344827586;0.0344827586, 0.0172413793, 0, 0.0172413793, 0.0344827586, 0.0172413793, 0.0344827586, 0.0517241379, 0.0172413793, 0.0172413793, 0.0344827586, 0, 0, 0.0344827586, 0, 0.0344827586, 0;0.0517241379, 0.0172413793, 0.0172413793, 0.0344827586, 0.0172413793, 0, 0, 0, 0.0344827586, 0, 0, 0.0172413793, 0.0689655172, 0.0862068966, 0.0172413793, 0.0517241379, 0.0172413793];
alphabet = 'DNA';
data_class = 'dna';
feature_type = 'Char';