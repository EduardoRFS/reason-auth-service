module Redis = Redis_lwt.Client;

let is_valid = Jwto.is_valid("kyouma_hououin");
let decode = Jwto.decode_and_verify("kyouma_hououin");
let encode = Jwto.encode(Jwto.HS256, "kyouma_hououin");

let connect = () => {
  let host = "localhost";
  let port = 6379;
  Redis.connect({host, port});
};
let conn_promise = connect();

let verify_token = jwto => {
  switch (encode(jwto)) {
  | Ok(string) =>
    Console.log(string);
    true;
  | Error(err) =>
    Console.error(err);
    false;
  };
};
let test = token => {
  let return =
    switch (Jwto.decode(token)) {
    | Ok(jwto) => true
    | Error(err) => false
    };
  Lwt.return(return);
};