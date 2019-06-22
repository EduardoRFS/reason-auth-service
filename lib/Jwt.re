open Base;

type header = string;
type payload = string;
type signature = string;
type t = {
  header,
  payload,
  signature,
};

let verify = t => true;
let decode = token => {
  let parts = String.split(token, ~on='.');
  switch (parts) {
  | [header, payload, signature] => Ok({signature, payload, header})
  | _ => Error("Invalid token")
  };
};