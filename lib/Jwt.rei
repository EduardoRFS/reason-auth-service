type t;
type header;
type payload;
type signature;
let verify: t => bool;
let decode: string => result(t, string);