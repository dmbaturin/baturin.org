#!/usr/bin/env ocaml

#use "topfind";;
#require "unix";;
#require "batteries";;
#require "fileutils";;
#require "lambdasoup";;

module FU = FileUtil
module FP = FilePath

open Soup.Infix

let (+/) left right =
    FP.concat left right

let exec str = ignore @@ Sys.command str

let doctype = "<!DOCTYPE html>"

let default_title = "Daniil Baturin"

let build_dir = "build"
let site_dir = "site"
let includes_dir = "includes"

let sections_file = "templates/sections.inc"
let index_file = "index.html"
let index_tmpl = "index.inc"
let main_tmpl = "templates/main.mpp"

let base_path = "/"
let tmpl_ext = "inc"

let list_dirs path =
    FU.filter FU.Is_dir (FU.ls path)

let list_inc_files path =
    FU.filter (FU.Has_extension tmpl_ext) (FU.ls path)

let to_file name str =
    let channel = open_out name in
    Printf.fprintf channel "%s" str;
    flush channel;
    close_out channel

let make_breadcrumbs nav_path =
    let rec aux xs acc_html acc_href =
        match xs with
          [] -> acc_html
        | x :: xs ->
           if x = "" then aux xs acc_html acc_href else
           let acc_href = Printf.sprintf "%s/%s" acc_href x in
           let link_html = Printf.sprintf "<a href=\"%s\">%s</a>" acc_href x in
           let acc_html = Printf.sprintf "%s / %s" acc_html link_html in
           aux xs acc_html acc_href
    in aux (List.rev nav_path) "" "" |> Printf.sprintf "<span>..%s /</span>"

let add_breadcrumbs nav_path soup =
    if List.length nav_path < 1 then Soup.delete (soup $ "#breadcrumbs")
    else let breadcrumbs_html = make_breadcrumbs nav_path in
    let breadcrumbs_node = Soup.parse breadcrumbs_html in
    Soup.append_child (soup $ "#breadcrumbs") breadcrumbs_node

let set_title soup =
    let title_string =
        try 
            let h1 = soup $ "h1" |> Soup.leaf_text |> BatOption.default "" in
            Printf.sprintf "%s &mdash; %s" h1 default_title
        with _ -> default_title
    in
    let title_node = Printf.sprintf "<title>%s</title>" title_string |> Soup.parse in
    Soup.replace (soup $ "title") title_node

(* Functions for processing section dirs *)

let safe_tl xs =
    match xs with
    | [] -> []
    | _ :: xs' -> xs'

let process_template main toc target_dir nav_path page =
    let page_name = FP.chop_extension (FP.basename page) in
    let target_dir =
        match page_name with
        | "index" -> target_dir
        | s -> target_dir +/ s
    in
    let target_file = target_dir +/ index_file in
    let mpp_command =
        Printf.sprintf "$MPP -its -soc {--- -scc ---}  -so {%% -sc %%} -son {{%% -scn %%}} -set page=\"%s\" -set toc=\"%s\" %s > %s"
            page toc main target_file
    in begin
        exec @@ "mkdir -p " ^ target_dir;
        exec mpp_command;

        let soup = Soup.read_file target_file |> Soup.parse in
        let () = if page_name <> "index" then add_breadcrumbs nav_path soup else add_breadcrumbs (safe_tl nav_path) soup in
        let () = set_title soup in
        Soup.to_string soup |> Printf.sprintf "%s\n%s" doctype |> Soup.write_file target_file
    end

let rec process_dir src_dir dst_dir nav_path dirname =
    let src_path = src_dir +/ dirname in
    let dst_path = dst_dir +/ dirname in
    let nav_path = if dirname <> "" then dirname :: nav_path else nav_path in
    let pages = list_inc_files src_path in
    let dirs = List.map (FP.basename) (list_dirs src_path) in
    List.iter (process_template main_tmpl sections_file dst_path nav_path) pages;
    List.iter (process_dir src_path dst_path nav_path) dirs


let () = exec @@ Printf.sprintf "cp -r %s/* %s/" includes_dir build_dir

let () = process_dir site_dir build_dir [] ""
