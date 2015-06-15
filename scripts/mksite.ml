#!/usr/bin/env ocaml

#use "topfind";;
#require "unix";;
#require "fileutils";;

module FU = FileUtil
module FP = FilePath

let (+/) left right =
    FP.concat left right

let exec str = ignore @@ Sys.command str

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

(* Functions for processing section dirs *)

let process_template main toc target_dir page =
    let page_name = FP.chop_extension (FP.basename page) in
    let target_dir =
        match page_name with
        | "index" -> target_dir
        | s -> target_dir +/ s
    in let mpp_command =
        Printf.sprintf "$MPP -its -so {%% -sc %%} -son {{%% -scn %%}} -set page=\"%s\" -set toc=\"%s\" %s > %s"
            page toc main (target_dir +/ index_file)
    in begin
        exec @@ "mkdir -p " ^ target_dir;
        exec mpp_command
    end

let rec process_dir src_dir dst_dir dirname =
    let src_path = src_dir +/ dirname in
    let dst_path = dst_dir +/ dirname in
    let pages = list_inc_files src_path in
    let dirs = List.map (FP.basename) (list_dirs src_path) in
    List.iter (process_template main_tmpl sections_file dst_path) pages;
    List.iter (process_dir src_path dst_path) dirs


let () = exec @@ Printf.sprintf "cp -r %s/* %s/" includes_dir build_dir

let () = process_dir site_dir build_dir ""
