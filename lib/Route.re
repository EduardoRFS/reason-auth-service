open Util;
open Opium.Std;

let get_token = req => {
  let%map body = App.string_of_body_exn(req);
  let json = Yojson.Basic.from_string(body);
  Yojson.Basic.Util.(json |> member("token") |> to_string);
};
let validate =
  post("/validate", req => {
    let%bind token = get_token(req);
    let%bind is_valid = Token.test(token);
    `String(is_valid ? "true" : "false") |> respond';
  });