# Analogy recovery #

An implementation of Levy and Goldberg's paper of recovering analogies between words from corpus. Wikiparser program is for filtering the Wikicorpus before processing it with the AnalogyRecovery software. After this processing analogies can be drawn using the functions Corpus::analogy_3_cos_mul and Corpus::analogy_3_cos_add - check test.cpp for further help.

### References ###

O. Levy and Y. Goldberg, “Linguistic Regularities in Sparse and Explicit Word Representations,” Proceedings of the Eighteenth Conference on Computational Natural Language Learning, pp. 171–180, 2014.

Samuel Reese, Gemma Boleda, Montse Cuadros, Lluís Padró, German Rigau. Wikicorpus: A Word-Sense Disambiguated Multilingual Wikipedia Corpus. In Proceedings of 7th Language Resources and Evaluation Conference (LREC'10), La Valleta, Malta. May, 2010.