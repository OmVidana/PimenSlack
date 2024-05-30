const modpow = (base, power, mod) => {
    let result = 1;
    for (let i = 0; i < power; i++) {
        result = (result * base) % mod;
    }
    return result;
};

export const encrypt = (inputStr, n, e) => {
    let ciphertext = "";
    for (let i = 0; i < inputStr.length; i++) {
        let c = modpow(inputStr.charCodeAt(i), e, n);
        ciphertext += c + " ";
    }
    ciphertext = ciphertext.trim();
    return ciphertext;
};