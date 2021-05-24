#lang racket/gui
"""
    Name: Daniel Schlatter
    Date: 8/2/19
    Class: CSCI 3415
    Description: Program to take a csv file of meteorite data and map it onto an image of earth. 
"""
(require csv-reading)
;;; Filenames
(define map-filename "Earth.EOSVid.jpg")
(define meteorites-filename "meteorite-landings.csv")

;;; Minimum and maximum latitude and longitude values
(define-values (lat-min lat-max) (values -90.0 90.0))
(define-values (long-min long-max) (values -180.0 180.0))

;;; Some data counts
(define fell-n (make-parameter 0))
(define found-n (make-parameter 0))
(define other-n (make-parameter 0))
(define invalid-n (make-parameter 0))
(define nomatch-n (make-parameter 0))

;;; (lat-long->x-y bitmap lat long) -> (values real? real?)
;;;   bitmap : (is-a?/c bitmap%
;;;   lat : (real-in -90.0 90.0)
;;;   long : (real-in -180.0 180.0)
;;; Returns the (x, y) coordinates corresponding to the given lat and long.
(define/contract (lat-long->x-y bitmap lat long)
  (-> (is-a?/c bitmap%) (real-in -90.0 90.0) real? ; (real-in -180.0 180.0)
      (values real? real?))
  (define width (send bitmap get-width))
  (define height (send bitmap get-height))
  (values (* width (/ (- long long-min) (- long-max long-min)))
          (- height (* height (/ (- lat lat-min) (- lat-max lat-min))) 1)))

;;; (fall->color fall) -> string?
;;;   fall : string?
;;; Returns the color used to render a specified fall value. Also increments
;;; the data count dynamic variables.
(define/contract (fall->color fall)
  (-> string? string?)
  (case fall
    (("Fell")
     (fell-n (+ (fell-n) 1)) ; Increment fell count
     "red")
    (("Found")
     (found-n (+ (found-n) 1)) ; Increment found count
     "green")
    (else
     (other-n (+ (other-n) 1)) ; Increment other count
     "white")))

;;; (main) -> any
(define (main)  ;;; Initialize data count dynamic variables.
  (parameterize ((fell-n 0)
                 (found-n 0)
                 (other-n 0)
                 (invalid-n 0)
                 (nomatch-n 0))
    ;; Load the world map bitmap
    (define map (make-object bitmap% 1024 512))
    (send map load-file map-filename)
    ;; And draw it on the bitmap
    (send bitmap-dc draw-bitmap map 0 0)
    (send canvas refresh-now)
    ;; Parse the meteorite landings file and skip the first row
    (define parsed-meteorite-landings
      (csv->list (file->string meteorites-filename)))
    (define meteroite-landings (cdr parsed-meteorite-landings))
    (printf "There are ~s meteorite landings in file ~s.~n"
            (length meteroite-landings) meteorites-filename)
    ;; Draw latitude and longitude lines
    (send bitmap-dc set-alpha 0.4)
    (send bitmap-dc set-smoothing 'smoothed)
    (send bitmap-dc set-pen "white" 1 'solid)
    (for ((long (in-range -180.0 180.0 10.0)))
      (define-values (x y) (lat-long->x-y bitmap -90 long))
      (send bitmap-dc draw-line x 0 x y))
    (for ((lat (in-range -90.0 90.0 10.0)))
      (define-values (x y) (lat-long->x-y bitmap lat 180))
      (send bitmap-dc draw-line 0 y x y))
    ;;; thicker lines for equator and prime meridian
    (send bitmap-dc set-pen "white" 3 'solid)
    (send bitmap-dc draw-line 512 0 512 512)
    (send bitmap-dc draw-line 0 256 1024 256)
    (send bitmap-dc set-smoothing 'aligned)
    ;; Iterate over all the meteorite landings and put them on the map
    (for ((landing (in-list meteroite-landings)))
      (match landing
        ((list name id nametype recclass mass fall year reclat reclong GeoLocation)
         ;;; create the logged mass width value to proportionally scale the meteorite representations. 
         (when (or (equal? mass "0") (equal? mass "")) ( set! mass "1"))
         (define massint (string->number mass))
         (set! massint (log massint))
         (when (< massint 3) (set! massint 3))
         
         (define lat (string->number reclat))
         (define long (string->number reclong))
         (cond ((and lat long)
                (send bitmap-dc set-pen (fall->color fall) massint 'solid)
                (send bitmap-dc set-alpha 0.5)
                (define-values (x y) (lat-long->x-y bitmap lat long))
                ;;; skip the meteors at 0,0
                (unless (and (= lat 0) (= long 0))     
                       (send bitmap-dc draw-point x y)))
               (else
                (invalid-n (+ (invalid-n) 1)))))
        (_
         (nomatch-n (+ (nomatch-n) 1))
         (void))))
    (send canvas refresh-now)
    ;; Print the data counts.
    (printf "Fell    = ~a~n" (fell-n))
    (printf "Found   = ~a~n" (found-n))
    (printf "Other   = ~a~n" (other-n))
    (printf "Invalid = ~a~n" (invalid-n))
    (printf "Nomatch = ~a~n" (nomatch-n))))

;;; Graphical Elements

(define frame
  (instantiate frame%
    ("Meteorites")))

(define menu-bar
  (instantiate menu-bar%
    (frame)))

(define file-menu
  (instantiate menu%
    ("&File" menu-bar)))

(define exit-menu-item
  (instantiate menu-item%
    ("E&xit" file-menu)
    (callback
     (lambda (menu-item event)
       (send frame show #f)))))

(define bitmap (make-object bitmap% 1024 512))
(define bitmap-dc (make-object bitmap-dc% bitmap))

(define canvas
  (instantiate canvas%
    (frame)
    (paint-callback (lambda (c dc)
                      (send dc draw-bitmap bitmap 0 0)))
    (style '(border))
    (min-width 1024)
    (min-height 512)))

(send frame show #t)

(main)
