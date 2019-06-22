open Base;
open Base.Result.Let_syntax;

module Yojson = Yojson.Safe;

type secret = string;

type root = {
  header: string,
  payload: string,
};
type header = Yojson.t;
type payload = Yojson.t;
type algorithm = string => Cryptokit.hash;
type signature = string;
// TODO: perhaps keep algorithm here
[@deriving fields]
type t = {
  root,
  header,
  payload,
  algorithm,
  signature,
};

// TODO: search about this wrong padding thing
// pad seems to be the = at the end
let (>>) = (g, f, x) => f(g(x));
let map_error = Base.Result.map_error(~f=(`Msg(error)) => error);
let encode_base64 = Base64.encode(~pad=false, ~alphabet=Base64.uri_safe_alphabet) >> map_error;
let decode_base64 = Base64.decode(~pad=false, ~alphabet=Base64.uri_safe_alphabet) >> map_error;

let decode_algorithm = header => {
  let member = Yojson.Util.member("alg", header);
  let%bind alg =
    switch (Yojson.Util.to_string_option(member)) {
    | Some(alg) => Ok(alg)
    | None => Error("Token header doesn't have an algorithm")
    };
  switch (alg) {
  | "HS256" => Ok(Cryptokit.MAC.hmac_sha256)
  | "HS512" => Ok(Cryptokit.MAC.hmac_sha512)
  | _ => Error("This module doesn't known " ++ alg)
  };
};
let decode_json = base64 => {
  let%map string = decode_base64(base64);
  Yojson.from_string(string);
};
let encode_json = json => Yojson.to_string(json) |> encode_base64;
let decode_token = token => {
  let parts = String.split(~on='.', token);
  switch (parts) {
  | [header, payload, signature] => Ok((header, payload, signature))
  | _ => Error("Invalid token")
  };
};
// perhaps doesn't make parse to allow fast verify
let decode = token => {
  let%bind (header, payload, signature) = decode_token(token);
  let root = {header, payload};
  let%bind header = decode_json(header);
  let%bind payload = decode_json(payload);
  let%map algorithm = decode_algorithm(header);
  {root, header, payload, algorithm, signature};
};
let sign = (~secret, t) => {
  let hash = t.algorithm(secret);
  let header = t.root.header;
  let payload = t.root.payload;
  let data = header ++ "." ++ payload;
  Cryptokit.hash_string(hash, data) |> encode_base64;
};
let verify = (~secret, t) =>
  switch (sign(~secret, t)) {
  | Ok(signature) => String.equal(signature, t.signature)
  | Error(_) => false
  };
let encode = (~secret, header, payload) => {
  let%bind root = {
    let%bind header = encode_json(header);
    let%map payload = encode_json(payload);
    { header, payload };
  };
  let%bind algorithm = decode_algorithm(header);
  let proto_t = {root, header, payload, algorithm, signature: ""};
  let%map signature = sign(~secret, proto_t);
  let token = root.header ++ "." ++ root.payload ++ "." ++ signature;
  ({...proto_t, signature}, token);
};
let secret_of_string = secret => secret;

let header_of_t = header;
let payload_of_t = payload;