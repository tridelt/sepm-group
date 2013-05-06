class Dashing.Log extends Dashing.Widget
  ready: ->
    $(@node).bind("DOMSubtreeModified", (e) =>
      @markup(e)
    )

    if @get('unordered')
      $(@node).find('ol').remove()
    else
      $(@node).find('ul').remove()

  markup: (e) =>
    $(@node).find('.label').each( (i, e) ->
      if $(e).text().indexOf('[ERROR]') > 0
        $(e).addClass('log-error')
        $(e.parentNode.childNodes[3]).addClass('log-error')
      if $(e).text().indexOf('[WARN]') > 0
        $(e).addClass('log-warning')
        $(e.parentNode.childNodes[3]).addClass('log-warning')
    )


