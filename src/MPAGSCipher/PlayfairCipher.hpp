#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include "CipherMode.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <map>

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher {
  public:
    /**
     * \brief Create a new PlayfairCipher with the given key
     *
     * \param key the key to use in the cipher
     */
    explicit PlayfairCipher(const std::string& key);

    /**
     * \brief set the key
     *
     * \param key the string to convert into the key to be used in the cipher
     */
    void setKey(const std::string& key);

    /**
     * \brief Apply the cipher to the provided text
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher(const std::string& inputText,
                            const CipherMode cipherMode) const;

  private:
    /// The alphabet 
    const std::string alphabet_{
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    // maps for use in applyCipher
    using PlayfairMap1 = std::map<char , std::pair<int,int>>;
    using PlayfairMap2 =  std::map<std::pair<int,int>, char> ;
    // create instance of map
    PlayfairMap1 map_letter_to_coords;
    PlayfairMap2 map_coords_to_letters;
    /// The cipher key
    std::string key_{""};
    int gridsize{5};
};

#endif