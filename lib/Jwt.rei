module Yojson = Yojson.Safe;

type t;
type header = Yojson.t;
type payload = Yojson.t;

type secret;

let decode: string => result(t, string);
let encode:
  (~secret: secret, header, payload) => result((t, string), string);
let verify: (~secret: secret, t) => bool;

let header_of_t: t => Yojson.t;
let payload_of_t: t => Yojson.t;

let secret_of_string: string => secret;