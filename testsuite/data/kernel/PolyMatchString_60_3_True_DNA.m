accuracy = 1e-10;
init_random = 42;
name = 'PolyMatchString';
data_type = '';
km_train = [1, 0.00967922425, 0.0180455633, 0.0256937805, 0.0302183883, 0.0180455633, 0.0120891176, 0.0256937805, 0.0256937805, 0.00967922425, 0.00967922425;0.00967922425, 1, 0.0180455633, 0.0256937805, 0.0352452408, 0.00761297201, 0.0180455633, 0.0044056551, 0.014869086, 0.00586392694, 0.00967922425;0.0180455633, 0.0180455633, 1, 0.0216449835, 0.0216449835, 0.0120891176, 0.0120891176, 0.0216449835, 0.00967922425, 0.00967922425, 0.00967922425;0.0256937805, 0.0256937805, 0.0216449835, 1, 0.0180455633, 0.0408007719, 0.00586392694, 0.00321172257, 0.0256937805, 0.0180455633, 0.014869086;0.0302183883, 0.0352452408, 0.0216449835, 0.0180455633, 1, 0.0120891176, 0.0408007719, 0.00586392694, 0.014869086, 0.014869086, 0.0352452408;0.0180455633, 0.00761297201, 0.0120891176, 0.0408007719, 0.0120891176, 1, 0.0180455633, 0.00967922425, 0.0180455633, 0.0352452408, 0.0120891176;0.0120891176, 0.0180455633, 0.0120891176, 0.00586392694, 0.0408007719, 0.0180455633, 1, 0.0180455633, 0.0256937805, 0.0120891176, 0.0408007719;0.0256937805, 0.0044056551, 0.0216449835, 0.00321172257, 0.00586392694, 0.00967922425, 0.0180455633, 1, 0.014869086, 0.0256937805, 0.0216449835;0.0256937805, 0.014869086, 0.00967922425, 0.0256937805, 0.014869086, 0.0180455633, 0.0256937805, 0.014869086, 1, 0.00967922425, 0.0256937805;0.00967922425, 0.00586392694, 0.00967922425, 0.0180455633, 0.014869086, 0.0352452408, 0.0120891176, 0.0256937805, 0.00967922425, 1, 0.014869086;0.00967922425, 0.00967922425, 0.00967922425, 0.014869086, 0.0352452408, 0.0120891176, 0.0408007719, 0.0216449835, 0.0256937805, 0.014869086, 1];
data_train = {'TTGCCCCAGCGCCATGAGTTGAGTGTCATGACTAACTTGGCATACGAGGGACGTATGTAC', 'ACTAGAGAACTAAATGGGAGACGGAATGAATTTCCAGAAAGCGTGCTCATAGCGGAGTCC', 'CTGTAAAAGTCTATATATACCCGTGGAGCTCGACCCCATATAAGGCGCGGCTGCTTGATA', 'ACGTGTAAGTGAAACATGGAAGTGTTAAGCCCCTACGGGTTGACGTTGGAAAACCGGGCC', 'TATCGAACGCGCCTTGAATATCTCCGTTGTGTTCGGGTCACACTGTATGAGTAGTAGCCG', 'ATACGCCTTTGTTCCACTGGAACTCTAGGTAAATAAGGTTGTGCTGCGCAGTGATACCAC', 'GTTTCGTCTCCGGGCGTTAAGCACTTCCGAGACCGGCACGAAGTCCTTCTGTCTTCTCAC', 'TGGGACGTACAACGAATTCGCTGTAGGCCGGAGAAGCCCGCTCGCGGCCCAAGTCCAATG', 'ACTACGGCTAGAGTCGTAGTGAACAGAACCGGAAACAGATCCTGGTCTCTCCGAACGTGT', 'GAAGCACGCTGGTCGTGTCGATTTGAATCCGCGCGTACCTCCGCTTGGAGGCATCGACTC', 'GACTGTTTTATAAACGCACCTCACCGCCCCTATGATTGCGCATTATGACCTTAGTTAGTC'};
feature_class = 'string';
seqlen = 60;
data_test = {'CTATCAAAAGATGCCGCATCAGGCTGGCTCGCGAATCCGGAATGCTGAACTAATAGAGCA', 'GGGGGGGGGGAACTTGCATTATCCGGTACCCGCCCGGGACAACAGTATAGGTACACTTGG', 'TCATCAAGGCGTTGTTCCCACCAACGTGCCAATCTGAGCCGTTTAAACCGGTTATCCTAT', 'GTTGAACATCTGACCCGAGCTTAAGTCCACCCGCACTCTGCAGGGTGATGCGGACCCAAA', 'CTACCTAAATGACAATCGCGCCGAGTATACGGATTATGTATATGCATGCCTCATCCATAG', 'TATCGCGCATATACTATCCGCCGCATTCGGGCTAGTGTCCTCGATGCCGAGGATCCCGTA', 'CATCGCTGTAGTGATCACGTCCGCTTCATAAAATGTACTTGATCGGGGGACGCCACTCGG', 'ATCTAACCTAGATTCAGAAGTTGGTGCTAAGACCGTACAGGCAGGACCTTGTGTACGGAC', 'CCCGTGTTCCATAGCTCTCCGTGTTTCCATTGCATATCCTACTGTTAACCCTAGAACTAG', 'GAGTGAGGAAAACCCTGTGACTAACCCACGCGGGAGACGACAACTCCGGTTTTAGTGTAC', 'TCGAGTCAATGAATTACTGTCGGTCACTCCGAACGGTTCGAAACACGCGCAAAAGTCCTA', 'AGGGGACATTCACACGTCAAAATATGGCGCCCTCCCAAAGCTGAGGGGGAGCGATCGTTG', 'ATGGTAAGTCGCGTAGTCCATCCCCGTCGGCATGGATGTTTTATATCGATACTGACAGTG', 'ACTAGCTTCGGCCGATAACACCGTTATCCCCTCGATTCGGGGGAGCCTACAACTCGAGTC', 'TTGCGTACCCTCCGAAGCCAGTTAGTCTTACAATTAGGCGTAAACCCGTCCTTACTACCA', 'ACTTTAATGCACCATATTCGGACGGGCCCCGTGGGGGATACAATCTCCCGTCCTACCACA', 'TGTGGGTACGTTGAATCATAGGACAGCATCCAATCCTGCTGAGTCGTGAAGCTCCGGCAG'};
kernel_arg0_degree = 3;
km_test = [0.0256937805, 0.0180455633, 0.0120891176, 0.0256937805, 0.0408007719, 0.0180455633, 0.0352452408, 0.00967922425, 0.0120891176, 0.0216449835, 0.0256937805, 0.0408007719, 0.0120891176, 0.0180455633, 0.0302183883, 0.0256937805, 0.0256937805;0.00321172257, 0.014869086, 0.0120891176, 0.00967922425, 0.0120891176, 0.0256937805, 0.00761297201, 0.0352452408, 0.00761297201, 0.0120891176, 0.0180455633, 0.0302183883, 0.0120891176, 0.0302183883, 0.0044056551, 0.0256937805, 0.0120891176;0.0180455633, 0.0180455633, 0.0216449835, 0.0408007719, 0.0352452408, 0.00761297201, 0.014869086, 0.0216449835, 0.0120891176, 0.0256937805, 0.0469114155, 0.0352452408, 0.0120891176, 0.00967922425, 0.0180455633, 0.0256937805, 0.00967922425;0.0256937805, 0.0180455633, 0.00321172257, 0.0120891176, 0.0302183883, 0.0180455633, 0.0180455633, 0.014869086, 0.0044056551, 0.0256937805, 0.0352452408, 0.0609037761, 0.0216449835, 0.0352452408, 0.0256937805, 0.014869086, 0.0352452408;0.00967922425, 0.0688383609, 0.0469114155, 0.00586392694, 0.00761297201, 0.0469114155, 0.0216449835, 0.0180455633, 0.00586392694, 0.0120891176, 0.0256937805, 0.0302183883, 0.0469114155, 0.0120891176, 0.0256937805, 0.0408007719, 0.0216449835;0.00321172257, 0.00321172257, 0.0352452408, 0.014869086, 0.0256937805, 0.0302183883, 0.0408007719, 0.0256937805, 0.0216449835, 0.0180455633, 0.0216449835, 0.0302183883, 0.0180455633, 0.00761297201, 0.0216449835, 0.00967922425, 0.0216449835;0.0216449835, 0.014869086, 0.0302183883, 0.0256937805, 0.00967922425, 0.0180455633, 0.0256937805, 0.0536036056, 0.0256937805, 0.014869086, 0.0120891176, 0.0256937805, 0.0216449835, 0.0180455633, 0.0256937805, 0.0352452408, 0.0352452408;0.0408007719, 0.014869086, 0.014869086, 0.0216449835, 0.0302183883, 0.0609037761, 0.0044056551, 0.0216449835, 0.0302183883, 0.0216449835, 0.0469114155, 0.0302183883, 0.0216449835, 0.0469114155, 0.0216449835, 0.0536036056, 0.00967922425;0.0180455633, 0.0536036056, 0.0180455633, 0.0256937805, 0.0180455633, 0.00761297201, 0.0302183883, 0.0536036056, 0.0180455633, 0.0256937805, 0.00967922425, 0.0408007719, 0.0256937805, 0.00967922425, 0.00225569541, 0.0256937805, 0.0256937805;0.0302183883, 0.014869086, 0.0180455633, 0.0408007719, 0.0302183883, 0.0302183883, 0.00967922425, 0.0302183883, 0.014869086, 0.0302183883, 0.0302183883, 0.0256937805, 0.0120891176, 0.0302183883, 0.00967922425, 0.0180455633, 0.014869086;0.0609037761, 0.0180455633, 0.0352452408, 0.0408007719, 0.0216449835, 0.0120891176, 0.014869086, 0.0216449835, 0.0469114155, 0.0352452408, 0.0408007719, 0.0180455633, 0.0469114155, 0.0352452408, 0.014869086, 0.0216449835, 0.0256937805];
kernel_arg1_inhomogene = 'True';
alphabet = 'DNA';
data_class = 'dna';
feature_type = 'Char';