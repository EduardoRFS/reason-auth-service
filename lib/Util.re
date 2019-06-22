module Let_syntax = {
  let bind = (~f, p) => Lwt.bind(p, f);
  let map = (~f, p) => Lwt.map(f, p);
};